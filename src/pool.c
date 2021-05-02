#include <assert.h>
#include <encin.h>
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <threads.h>
#include <ucontext.h>
#include <unistd.h>


static thread_local encin_stack_size stack_size_to_be_released = 0;

static thread_local void *stack_to_be_released = NULL;

static thread_local ucontext_t releaser_context = { 0 };


static thread_local ucontext_t worker_context = { 0 };

static thread_local encin_job *active_job = NULL;

static thread_local pthread_mutex_t *mutex_to_unlock = NULL;

static void *worker(void *argument) {
    (void)argument;

    getcontext(&releaser_context);
    encin_stack_release(stack_size_to_be_released, stack_to_be_released);

    stack_size_to_be_released = 0;
    stack_to_be_released = NULL;

    getcontext(&worker_context);
    if (mutex_to_unlock != NULL) {
        pthread_mutex_unlock(mutex_to_unlock);
        mutex_to_unlock = NULL;
    }

    active_job = encin_queue_pop(&encin_job_queue);
    if (active_job == NULL) {
        return NULL;
    }

    encin_status = active_job->encin_status_;
    errno = active_job->errno_;

    active_job->is_scheduled = true;
    setcontext(&active_job->context);

    return NULL;
}


struct timespec timeout = {
    .tv_sec = ENCIN_BLOCKING_POOL_TIMEOUT / 1000,
    .tv_nsec = (ENCIN_BLOCKING_POOL_TIMEOUT % 1000) * 1000000,
};

static size_t blocking_thread_count = 0;

static size_t idle_blocking_thread_count = 0;

static bool shutting_down = false;

static void *blocking_worker(void *argument) {
    (void)argument;

    pthread_mutex_t *lock = &encin_blocking_job_queue.lock;
    pthread_cond_t *cv = &encin_blocking_job_queue.cv;

    pthread_mutex_lock(lock);
    while (true) {
        if (shutting_down) {
            break;
        }

        idle_blocking_thread_count -= 1;
        while (true) {
            active_job = encin_queue_try_pop(&encin_blocking_job_queue);
            if (active_job == NULL) {
                break;
            } else {
                getcontext(&releaser_context);

                encin_stack_release(stack_size_to_be_released, stack_to_be_released);
                stack_size_to_be_released = 0;
                stack_to_be_released = NULL;

                getcontext(&worker_context);
                if (mutex_to_unlock != NULL) {
                    pthread_mutex_unlock(mutex_to_unlock);
                    mutex_to_unlock = NULL;
                }

                if (active_job != NULL) {
                    pthread_mutex_unlock(lock);

                    encin_status = active_job->encin_status_;
                    errno = active_job->errno_;

                    active_job->is_scheduled = true;
                    setcontext(&active_job->context);
                } else {
                    pthread_mutex_lock(lock);
                }
            }
        }
        idle_blocking_thread_count += 1;

        int result = pthread_cond_timedwait(cv, lock, &timeout);
        if (result == ETIMEDOUT && encin_queue_length(&encin_blocking_job_queue) == 0) {
            break;
        }
    }
    blocking_thread_count -= 1;
    idle_blocking_thread_count -= 1;

    pthread_mutex_unlock(lock);
    return NULL;
}


int encin_schedule(encin_job *job) {
    if (job == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (job->is_blocking) {
        int result;

        result = pthread_mutex_lock(&encin_blocking_job_queue.lock);
        if (result != 0) {
            errno = result;
            return -1;
        }

        if (encin_queue_push_without_locking(&encin_blocking_job_queue, job) != 0) {
            pthread_mutex_unlock(&encin_blocking_job_queue.lock);
            return -1;
        }

        switch (encin_blocking_pool_grow()) {
            case -1: {
                encin_queue_try_pop(&encin_blocking_job_queue);
                pthread_mutex_unlock(&encin_blocking_job_queue.lock);
                return -1;
            }
            case 0: {
                pthread_cond_signal(&encin_blocking_job_queue.cv);
                break;
            }
            case 1: {
                break;
            }
        }
        pthread_mutex_unlock(&encin_blocking_job_queue.lock);
    } else {
        if (encin_queue_push(&encin_job_queue, job) != 0) {
            return -1;
        }
    }

    return 0;
}

encin_job *encin_active_job() {
    return active_job;
}

void encin_deschedule(void) {
    assert(active_job != NULL);

    active_job->encin_status_ = encin_status;
    active_job->errno_ = errno;

    active_job->is_scheduled = false;
    asm volatile("" : : "r,m"(active_job->is_scheduled) : "memory");

    getcontext(&active_job->context);
    if (active_job->is_scheduled == false) {
        mutex_to_unlock = &active_job->lock;
        active_job = NULL;
        setcontext(&worker_context);
    }
}

void encin_finalize(encin_stack_size stack_size) {
    pthread_mutex_lock(&active_job->parent->lock);
    active_job->is_completed = true;
    if (active_job->parent->awaiting == active_job) {
        active_job->parent->awaiting = NULL;
        encin_schedule(active_job->parent);
    }
    pthread_mutex_unlock(&active_job->parent->lock);

    stack_size_to_be_released = stack_size;
    stack_to_be_released = active_job->context.uc_stack.ss_sp;

    active_job = NULL;
    setcontext(&releaser_context);
}

void encin_finalize_detached(encin_stack_size stack_size) {
    stack_size_to_be_released = stack_size;
    stack_to_be_released = active_job->context.uc_stack.ss_sp;

    pthread_mutex_destroy(&active_job->lock);
    free(active_job);

    active_job = NULL;
    setcontext(&releaser_context);
}


#if ENCIN_POOL_SIZE == 0

static size_t thread_count = 0;
static pthread_t *threads = NULL;

#else

static size_t thread_count = ENCIN_POOL_SIZE;
static pthread_t threads[ENCIN_POOL_SIZE] = { 0 };

#endif

int encin_pool_start(void) {
    #if ENCIN_POOL_SIZE == 0

    char *environment = getenv("ENCIN_POOL_SIZE");
    if (environment == NULL) {
        thread_count = sysconf(_SC_NPROCESSORS_ONLN);
    } else {
        char *cursor = environment;
        while (*cursor != '\0') {
            char character = *cursor;

            if (character >= '0' && character <= '9') {
                thread_count *= 10;
                thread_count += character - '0';
            } else {
                return -1;
            }
            if (thread_count > USHRT_MAX) {
                return -2;
            }

            cursor++;
        }
        if (thread_count == 0) {
            return -1;
        }
    }

    threads = malloc(thread_count * sizeof(pthread_t));
    if (threads == NULL) {
        return -1;
    }

    #endif

    int result;

    result = encin_queue_create(&encin_job_queue, ENCIN_JOB_QUEUE_INITIAL_CAPACITY);
    if (result == -1) {
        #if ENCIN_POOL_SIZE == 0
        free(threads);
        #endif

        return -3;
    }

    pthread_attr_t thread_attributes;
    result = pthread_attr_init(&thread_attributes);
    if (result != 0) {
        encin_queue_destroy(&encin_job_queue);

        #if ENCIN_POOL_SIZE == 0
        free(threads);
        #endif

        errno = result;
        return -4;
    }

    size_t stack_size = PTHREAD_STACK_MIN;
    result = pthread_attr_setstacksize(&thread_attributes, stack_size);
    if (result != 0) {
        pthread_attr_destroy(&thread_attributes);

        encin_queue_destroy(&encin_job_queue);

        #if ENCIN_POOL_SIZE == 0
        free(threads);
        #endif

        errno = result;
        return -4;
    }

    for (size_t i = 0; i < thread_count; i++) {
        result = pthread_create(&threads[i], &thread_attributes, worker, NULL);
        if (result != 0) {
            for (size_t j = 0; j < i; j++) {
                while (encin_queue_push(&encin_job_queue, NULL) == -1) {}
            }
            for (size_t j = 0; j < i; j++) {
                pthread_join(threads[j], NULL);
            }

            pthread_attr_destroy(&thread_attributes);

            encin_queue_destroy(&encin_job_queue);

            #if ENCIN_POOL_SIZE == 0
            free(threads);
            #endif

            errno = result;
            return -4;
        }
    }

    pthread_attr_destroy(&thread_attributes);
    return 0;
}

void encin_pool_stop(void) {
    for (size_t i = 0; i < thread_count; i++) {
        while (encin_queue_push(&encin_job_queue, NULL) == -1) {
            sched_yield();
        }
    }
    for (size_t i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    encin_queue_destroy(&encin_job_queue);

    #if ENCIN_POOL_SIZE == 0
    free(threads);
    #endif
}


int encin_blocking_pool_grow(void) {
    bool there_arent_enough_threads
        = idle_blocking_thread_count < encin_queue_length(&encin_blocking_job_queue);

    bool pool_size_limit_isnt_reached
        = blocking_thread_count < ENCIN_BLOCKING_POOL_SIZE_LIMIT;

    if (there_arent_enough_threads && pool_size_limit_isnt_reached) {

        // TODO: minimize stack

        pthread_t tid;
        int result = pthread_create(&tid, NULL, blocking_worker, NULL);
        if (result != 0) {
            if (blocking_thread_count == 0) {
                errno = result;
                return -1;
            }
        } else {
            blocking_thread_count += 1;
            idle_blocking_thread_count += 1;

            pthread_detach(tid);
            return 1;
        }
    }

    return 0;
}

void encin_blocking_pool_stop(void) {
    pthread_mutex_t *lock = &encin_blocking_job_queue.lock;
    pthread_cond_t *cv = &encin_blocking_job_queue.cv;

    pthread_mutex_lock(lock);
    shutting_down = true;
    pthread_cond_broadcast(cv);
    while (blocking_thread_count != 0) {
        pthread_mutex_unlock(lock);
        usleep(1000);
        pthread_mutex_lock(lock);
    }
    pthread_mutex_unlock(lock);

    encin_queue_destroy(&encin_blocking_job_queue);
}

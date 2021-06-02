#include <encin.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <ucontext.h>


static struct { int result; int argc; char **argv; char **envp; } environment = { 0 };

static ucontext_t context = { 0 };

static pthread_cond_t condvar = PTHREAD_COND_INITIALIZER;


extern int encin();

static void ignitor(void) {
    environment.result = encin(environment.argc, environment.argv, environment.envp);

    encin_job *job = encin_active_job();
    pthread_mutex_lock(&job->lock);

    pthread_cond_signal(&condvar);
    encin_deschedule();
}


int main(int argc, char *argv[], char *envp[]) {
    #define RED(string) "\x1B[31m" string "\x1B[0m"

    int result;

    result = encin_signal_start();
    if (result != 0) {
        switch (result) {
            case -1: {
                fprintf(
                    stderr,
                    RED("fatal encin error:")" signal file creation failed (%s)\n",
                    strerror(errno)
                );
                fflush(stderr);
                break;
            }
            case -2: {
                fprintf(
                    stderr,
                    RED("fatal encin error:")" signal listener creation failed (%s)\n",
                    strerror(errno)
                );
                fflush(stderr);
                break;
            }
            default: {
                __builtin_unreachable();
            }
        }
        goto ENCIN_SIGNAL_START_FAILED;
    }

    result = encin_blocking_pool_start();
    if (result != 0) {
        fprintf(
            stderr,
            RED("fatal encin error:")" encin blocking job queue creation failed (%s)\n",
            strerror(errno)
        );
        fflush(stderr);
        goto ENCIN_BLOCKING_POOL_START_FAILED;
    }

    result = encin_pool_start();
    if (result != 0) {
        switch (result) {
            case -1: {
                fprintf(
                    stderr,
                    RED("fatal encin error:")" encin pool is misconfigured (%s)\n",
                    "ENCIN_POOL_SIZE is not a positive number"
                );
                fflush(stderr);
                break;
            }
            case -2: {
                fprintf(
                    stderr,
                    RED("fatal encin error:")" encin pool is misconfigured (%s)\n",
                    "ENCIN_POOL_SIZE is too large"
                );
                fflush(stderr);
                break;
            }
            case -3: {
                fprintf(
                    stderr,
                    RED("fatal encin error:")" encin job queue creation failed (%s)\n",
                    strerror(errno)
                );
                fflush(stderr);
                break;
            }
            case -4: {
                fprintf(
                    stderr,
                    RED("fatal encin error:")" encin pool creation failed (%s)\n",
                    strerror(errno)
                );
                fflush(stderr);
                break;
            }
            default: {
                __builtin_unreachable();
            }
        }
        goto ENCIN_POOL_START_FAILED;
    }

    void *stack = encin_stack_acquire(ENCIN_MAIN_STACK_SIZE);
    if (stack == NULL) {
        fprintf(
            stderr,
            RED("fatal encin error:")" main stack allocation failed (%s)\n",
            strerror(errno)
        );
        fflush(stderr);
        goto ENCIN_STACK_ACQUIRE_FAILED;
    }

    environment.argc = argc;
    environment.argv = argv;
    environment.envp = envp;

    getcontext(&context);
    context.uc_stack.ss_sp = stack;
    context.uc_stack.ss_size = 1 << ENCIN_MAIN_STACK_SIZE;
    makecontext(&context, ignitor, 0);

    encin_job job = {

        .is_blocking = false,

        .encin_status_ = encin_status,
        .errno_ = errno,

        .is_scheduled = false,
        .context = context,

        .is_completed = false,
        .parent = NULL,
        .awaiting = NULL,
        .lock = PTHREAD_MUTEX_INITIALIZER,

    };

    pthread_mutex_lock(&job.lock);

    result = encin_schedule(&job);
    if (result == -1) {
        pthread_mutex_unlock(&job.lock);
        fprintf(
            stderr,
            RED("fatal encin error:")" entry point creation failed (%s)\n",
            strerror(errno)
        );
        fflush(stderr);
        goto ENCIN_SCHEDULE_FAILED;
    }

    pthread_cond_wait(&condvar, &job.lock);
    pthread_mutex_unlock(&job.lock);

    pthread_mutex_destroy(&job.lock);
    encin_stack_release(ENCIN_MAIN_STACK_SIZE, stack);
    encin_blocking_pool_stop();
    encin_pool_stop();
    encin_signal_stop();
    pthread_cond_destroy(&condvar);

    return environment.result;


    ENCIN_SCHEDULE_FAILED:
    pthread_mutex_destroy(&job.lock);
    encin_stack_release(ENCIN_MAIN_STACK_SIZE, stack);

    ENCIN_STACK_ACQUIRE_FAILED:
    encin_pool_stop();

    ENCIN_POOL_START_FAILED:
    encin_blocking_pool_stop();

    ENCIN_BLOCKING_POOL_START_FAILED:
    encin_signal_stop();

    ENCIN_SIGNAL_START_FAILED:
    pthread_cond_destroy(&condvar);

    return EX_OSERR;
}

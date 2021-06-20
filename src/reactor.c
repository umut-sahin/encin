#include <encin.h>
#include <errno.h>
#include <liburing.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>


pthread_t reactor_tid;

static void *reactor(void *argument) {
    (void)argument;

    struct io_uring_cqe *cqe;
    while (true) {
        int result;

        result = io_uring_wait_cqe(&encin_ring, &cqe);
        if (result == -EINTR) {
            continue;
        }

        int value = cqe->res;
        encin_io_job *job = io_uring_cqe_get_data(cqe);

        io_uring_cqe_seen(&encin_ring, cqe);

        if (job == NULL) {
            break;
        }

        pthread_mutex_t *parent_lock = &job->parent->lock;
        pthread_mutex_lock(parent_lock);
        job->result = value;
        job->is_completed = true;
        if (job->parent->awaiting == job) {
            job->parent->awaiting = NULL;
            encin_schedule(job->parent);
        }
        pthread_mutex_unlock(parent_lock);
    }

    return NULL;
}


struct io_uring encin_ring;

pthread_mutex_t encin_ring_lock = PTHREAD_MUTEX_INITIALIZER;

int encin_reactor_start() {
    int result;

    result = io_uring_queue_init(ENCIN_REACTOR_ENTRY_LIMIT, &encin_ring, 0);
    if (result != 0) {
        errno = -result;
        return -1;
    }

    // TODO: optimize stack size

    result = pthread_create(&reactor_tid, NULL, reactor, NULL);
    if (result != 0) {
        errno = result;
        return -2;
    }

    return 0;
}

void encin_reactor_stop() {
    pthread_mutex_lock(&encin_ring_lock);

    struct io_uring_sqe *sqe = io_uring_get_sqe(&encin_ring);
    io_uring_prep_nop(sqe);
    io_uring_sqe_set_data(sqe, NULL);
    io_uring_submit(&encin_ring);

    pthread_mutex_unlock(&encin_ring_lock);

    pthread_join(reactor_tid, NULL);
    io_uring_queue_exit(&encin_ring);
}

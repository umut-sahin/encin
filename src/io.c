#include <encin.h>

encin_io_job *encin_accept(int fd, struct sockaddr *address, socklen_t *address_length, int flags) {
    encin_io_job *job = malloc(sizeof(encin_io_job));

    job->is_completed = false;
    job->result = 0;
    job->parent = encin_active_job();

    pthread_mutex_lock(&encin_ring_lock);

    struct io_uring_sqe *sqe = io_uring_get_sqe(&encin_ring);
    io_uring_prep_accept(sqe, fd, address, address_length, flags);
    io_uring_sqe_set_data(sqe, job);
    io_uring_submit(&encin_ring);

    pthread_mutex_unlock(&encin_ring_lock);

    return job;
}

encin_io_job *encin_read(int fd, void *buffer, size_t count, off_t offset) {
    encin_io_job *job = malloc(sizeof(encin_io_job));

    job->is_completed = false;
    job->result = 0;
    job->parent = encin_active_job();

    pthread_mutex_lock(&encin_ring_lock);

    struct io_uring_sqe *sqe = io_uring_get_sqe(&encin_ring);
    io_uring_prep_read(sqe, fd, buffer, count, offset);
    io_uring_sqe_set_data(sqe, job);
    io_uring_submit(&encin_ring);

    pthread_mutex_unlock(&encin_ring_lock);

    return job;
}

encin_io_job *encin_write(int fd, void *buffer, size_t count, off_t offset) {
    encin_io_job *job = malloc(sizeof(encin_io_job));

    job->is_completed = false;
    job->result = 0;
    job->parent = encin_active_job();

    pthread_mutex_lock(&encin_ring_lock);

    struct io_uring_sqe *sqe = io_uring_get_sqe(&encin_ring);
    io_uring_prep_write(sqe, fd, buffer, count, offset);
    io_uring_sqe_set_data(sqe, job);
    io_uring_submit(&encin_ring);

    pthread_mutex_unlock(&encin_ring_lock);

    return job;
}

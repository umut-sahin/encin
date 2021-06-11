#pragma once

#include <encin/job.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct encin_io_job {
    bool is_completed;
    int result;
    encin_job *parent;
} encin_io_job;

encin_io_job *encin_accept(int fd, struct sockaddr *address, socklen_t *address_length, int flags);

encin_io_job *encin_read(int fd, void *buffer, size_t count, off_t offset);

encin_io_job *encin_write(int fd, void *buffer, size_t count, off_t offset);

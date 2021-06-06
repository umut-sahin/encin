#pragma once

#include <encin/job.h>
#include <stddef.h>

typedef struct encin_io_job {
    int result;
    encin_job *parent;
} encin_io_job;

encin_io_job *encin_read(int fd, void *buffer, size_t count);

encin_io_job *encin_write(int fd, void *buffer, size_t count);

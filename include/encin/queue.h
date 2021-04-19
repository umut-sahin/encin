#pragma once

#include <encin/job.h>
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>

typedef struct encin_queue {

    size_t capacity;
    size_t top;
    size_t bottom;
    encin_job **buffer;

    pthread_mutex_t lock;
    pthread_cond_t cv;

} encin_queue;

int encin_queue_create(encin_queue *queue, size_t capacity);

int encin_queue_push(encin_queue *queue, encin_job *job);

size_t encin_queue_length(encin_queue *queue);

encin_job *encin_queue_pop(encin_queue *queue);

encin_job *encin_queue_try_pop(encin_queue *queue);

void encin_queue_destroy(encin_queue *queue);


#ifndef ENCIN_JOB_QUEUE_INITIAL_CAPACITY
#define ENCIN_JOB_QUEUE_INITIAL_CAPACITY (32768)
#endif
static_assert(
    ENCIN_JOB_QUEUE_INITIAL_CAPACITY >= 0,
    "ENCIN_JOB_QUEUE_INITIAL_CAPACITY is negative"
);
static_assert(
    ENCIN_JOB_QUEUE_INITIAL_CAPACITY <= SIZE_MAX,
    "ENCIN_JOB_QUEUE_INITIAL_CAPACITY is too large"
);

extern encin_queue encin_job_queue;


#ifndef ENCIN_BLOCKING_JOB_QUEUE_INITIAL_CAPACITY
#define ENCIN_BLOCKING_JOB_QUEUE_INITIAL_CAPACITY (32768)
#endif
static_assert(
    ENCIN_BLOCKING_JOB_QUEUE_INITIAL_CAPACITY >= 0,
    "ENCIN_BLOCKING_JOB_QUEUE_INITIAL_CAPACITY is negative"
);
static_assert(
    ENCIN_BLOCKING_JOB_QUEUE_INITIAL_CAPACITY <= SIZE_MAX,
    "ENCIN_BLOCKING_JOB_QUEUE_INITIAL_CAPACITY is too large"
);

extern encin_queue encin_blocking_job_queue;

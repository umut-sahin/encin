#pragma once

#include <encin/job.h>
#include <pthread.h>
#include <stddef.h>

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

encin_job *encin_queue_pop(encin_queue *queue);

void encin_queue_destroy(encin_queue *queue);

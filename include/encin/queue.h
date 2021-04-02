#pragma once

#include <encin/job.h>
#include <stddef.h>

typedef struct encin_queue {

} encin_queue;

int encin_queue_create(encin_queue *queue, size_t capacity);

int encin_queue_push(encin_queue *queue, encin_job *job);

encin_job *encin_queue_pop(encin_queue *queue);

void encin_queue_destroy(encin_queue *queue);

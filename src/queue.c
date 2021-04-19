#include <encin.h>
#include <errno.h>
#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


static int encin_queue_grow(encin_queue *queue) {
    if (queue->capacity > SIZE_MAX / 2) {
        errno = ENOMEM;
        return -1;
    }

    size_t new_capacity = queue->capacity * 2;
    encin_job **new_buffer = realloc(queue->buffer, new_capacity * sizeof(encin_job *));
    if (new_buffer == NULL) {
        return -1;
    }

    if (queue->bottom > queue->top) {
        memcpy(&new_buffer[queue->capacity], &new_buffer[0], queue->top * sizeof(encin_job *));
        queue->top = queue->capacity + queue->top;
    }

    queue->capacity = new_capacity;
    queue->buffer = new_buffer;

    return 0;
}


int encin_queue_create(encin_queue *queue, size_t capacity) {
    int result;

    result = pthread_mutex_init(&queue->lock, NULL);
    if (result != 0) {
        errno = result;
        return -1;
    }

    result = pthread_cond_init(&queue->cv, NULL);
    if (result != 0) {
        pthread_mutex_destroy(&queue->lock);
        errno = result;
        return -1;
    }

    queue->capacity = capacity;
    queue->top = 0;
    queue->bottom = 0;

    if (capacity != 0) {
        queue->buffer = malloc(capacity * sizeof(encin_job *));
        if (queue->buffer == NULL) {
            pthread_cond_destroy(&queue->cv);
            pthread_mutex_destroy(&queue->lock);
            return -1;
        }
    } else {
        queue->buffer = NULL;
    }

    return 0;
}

int encin_queue_push(encin_queue *queue, encin_job *job) {
    int result;

    result = pthread_mutex_lock(&queue->lock);
    if (result != 0) {
        errno = result;
        return -1;
    }

    if (queue->bottom <= queue->top) {
        if (queue->top == queue->capacity && queue->bottom == 0) {
            if (encin_queue_grow(queue) == -1) {
                pthread_mutex_unlock(&queue->lock);
                return -1;
            }
        }
    } else {
        if (queue->bottom - 1 == queue->top) {
            if (encin_queue_grow(queue) == -1) {
                pthread_mutex_unlock(&queue->lock);
                return -1;
            }
        }
    }

    queue->buffer[queue->top] = job;

    queue->top++;
    if (queue->top == queue->capacity) {
        queue->top = 0;
    }

    pthread_cond_signal(&queue->cv);
    pthread_mutex_unlock(&queue->lock);
    return 0;
}

size_t encin_queue_length(encin_queue *queue) {
    if (queue->bottom <= queue->top) {
        return queue->top - queue->bottom;
    } else {
        return queue->capacity - (queue->bottom - queue->top);
    }
}

encin_job *encin_queue_try_pop(encin_queue *queue) {
    if (queue->bottom == queue->top) {
        return NULL;
    }

    encin_job *job = queue->buffer[queue->bottom];

    queue->bottom++;
    if (queue->bottom == queue->capacity) {
        queue->bottom = 0;
    }

    return job;
}

encin_job *encin_queue_pop(encin_queue *queue) {
    int result;

    result = pthread_mutex_lock(&queue->lock);
    if (result != 0) {
        errno = result;
        return NULL;
    }

    while (queue->bottom == queue->top) {
        result = pthread_cond_wait(&queue->cv, &queue->lock);
        if (result != 0) {
            pthread_mutex_unlock(&queue->lock);
            errno = result;
            return NULL;
        }
    }

    encin_job *job = queue->buffer[queue->bottom];

    queue->bottom++;
    if (queue->bottom == queue->capacity) {
        queue->bottom = 0;
    }

    pthread_mutex_unlock(&queue->lock);
    return job;
}

void encin_queue_destroy(encin_queue *queue) {
    free(queue->buffer);
    pthread_cond_destroy(&queue->cv);
    pthread_mutex_destroy(&queue->lock);
}


encin_queue encin_job_queue;

encin_queue encin_blocking_job_queue;

#pragma once

#include <encin/io.h>
#include <encin/job.h>
#include <encin/pool.h>
#include <pthread.h>
#include <stdlib.h>

#define encin_await(task) ({                                                                       \
    encin_job *_encin_active_job = encin_active_job();                                             \
                                                                                                   \
    pthread_mutex_lock(&_encin_active_job->lock);                                                  \
    if (task->job.is_completed) {                                                                  \
        pthread_mutex_unlock(&_encin_active_job->lock);                                            \
    } else {                                                                                       \
        _encin_active_job->awaiting = (void *)task;                                                \
        encin_deschedule();                                                                        \
    }                                                                                              \
                                                                                                   \
    __auto_type _encin_result = task->result;                                                      \
    pthread_mutex_destroy(&task->job.lock);                                                        \
    free(task);                                                                                    \
    _encin_result;                                                                                 \
})

#define encin_await_io(task) ({                                                                    \
    encin_job *_encin_active_job = encin_active_job();                                             \
                                                                                                   \
    pthread_mutex_lock(&_encin_active_job->lock);                                                  \
    if (task->is_completed) {                                                                      \
        pthread_mutex_unlock(&_encin_active_job->lock);                                            \
    } else {                                                                                       \
        _encin_active_job->awaiting = (void *)task;                                                \
        encin_deschedule();                                                                        \
    }                                                                                              \
                                                                                                   \
    __auto_type _encin_result = task->result;                                                      \
    free(task);                                                                                    \
                                                                                                   \
    _encin_result < 0                                                                              \
    ? ({                                                                                           \
        errno = -_encin_result;                                                                    \
        -1;                                                                                        \
    })                                                                                             \
    : ({                                                                                           \
        _encin_result;                                                                             \
    });                                                                                            \
})

#define encin_await_void(task) ({                                                                  \
    encin_job *_encin_active_job = encin_active_job();                                             \
                                                                                                   \
    pthread_mutex_lock(&_encin_active_job->lock);                                                  \
    if (task->job.is_completed) {                                                                  \
        pthread_mutex_unlock(&_encin_active_job->lock);                                            \
    } else {                                                                                       \
        _encin_active_job->awaiting = (encin_job *)task;                                           \
        encin_deschedule();                                                                        \
    }                                                                                              \
                                                                                                   \
    pthread_mutex_destroy(&task->job.lock);                                                        \
    free(task);                                                                                    \
})

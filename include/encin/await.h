#pragma once

#include <encin/io.h>
#include <encin/job.h>
#include <encin/pool.h>
#include <pthread.h>
#include <stdlib.h>

#define encin_await(task)                                                                          \
    _Generic((task), encin_io_job: encin_await_io_job, default: encin_await_job)(task)

#define encin_await_io_job(task) ({                                                                \
    task->result;                                                                                  \
})

#define encin_await_job(task) ({                                                                   \
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
    __auto_type _encin_result = task->result;                                                      \
    pthread_mutex_destroy(&task->job.lock);                                                        \
    free(task);                                                                                    \
    _encin_result;                                                                                 \
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

#pragma once

#include <encin/result.h>
#include <pthread.h>
#include <stdbool.h>
#include <ucontext.h>

typedef struct encin_job {

    bool is_blocking;

    encin_result encin_status_;
    int errno_;

    bool is_scheduled;
    ucontext_t context;

    bool is_completed;
    struct encin_job *parent;
    void *awaiting;
    pthread_mutex_t lock;

} encin_job;

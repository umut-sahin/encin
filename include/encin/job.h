#pragma once

#include <encin/result.h>
#include <stdbool.h>
#include <ucontext.h>

typedef struct encin_job {

    bool is_blocking;

    encin_result encin_status_;
    int errno_;

    bool is_scheduled;
    ucontext_t context;

} encin_job;

#pragma once

#include <liburing.h>
#include <limits.h>
#include <pthread.h>

extern struct io_uring encin_ring;

extern pthread_mutex_t encin_ring_lock;

#ifndef ENCIN_REACTOR_ENTRY_LIMIT
#define ENCIN_REACTOR_ENTRY_LIMIT (1024)
#endif
static_assert(
    ENCIN_REACTOR_ENTRY_LIMIT >= 0,
    "ENCIN_REACTOR_ENTRY_LIMIT is negative"
);
static_assert(
    ENCIN_REACTOR_ENTRY_LIMIT <= UINT_MAX,
    "ENCIN_REACTOR_ENTRY_LIMIT is too large"
);

int encin_reactor_start();

void encin_reactor_stop();

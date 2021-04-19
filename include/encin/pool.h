#pragma once

#include <limits.h>

void encin_deschedule();


#ifndef ENCIN_POOL_SIZE
#define ENCIN_POOL_SIZE (0)
#endif
static_assert(
    ENCIN_POOL_SIZE >= 0,
    "ENCIN_POOL_SIZE is negative"
);
static_assert(
    ENCIN_POOL_SIZE <= USHRT_MAX,
    "ENCIN_POOL_SIZE is too large"
);

int encin_pool_start(void);

void encin_pool_stop(void);


#ifndef ENCIN_BLOCKING_POOL_SIZE_LIMIT
#define ENCIN_BLOCKING_POOL_SIZE_LIMIT (512)
#endif
static_assert(
    ENCIN_BLOCKING_POOL_SIZE_LIMIT >= 0,
    "ENCIN_BLOCKING_POOL_SIZE_LIMIT is negative"
);
static_assert(
    ENCIN_BLOCKING_POOL_SIZE_LIMIT <= USHRT_MAX,
    "ENCIN_BLOCKING_POOL_SIZE_LIMIT is too large"
);

#ifndef ENCIN_BLOCKING_POOL_TIMEOUT
#define ENCIN_BLOCKING_POOL_TIMEOUT (1000)
#endif
static_assert(
    ENCIN_BLOCKING_POOL_TIMEOUT >= 0,
    "ENCIN_BLOCKING_POOL_TIMEOUT is negative"
);
static_assert(
    ENCIN_BLOCKING_POOL_TIMEOUT <= UINT_MAX,
    "ENCIN_BLOCKING_POOL_TIMEOUT is too large"
);

int encin_blocking_pool_grow(void);

void encin_blocking_pool_stop(void);

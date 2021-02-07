#pragma once

typedef enum {

    ENCIN_STACK_16K = 14,
    ENCIN_STACK_32K = 15,
    ENCIN_STACK_64K = 16,
    ENCIN_STACK_128K = 17,
    ENCIN_STACK_256K = 18,
    ENCIN_STACK_512K = 19,

    ENCIN_STACK_1M = 20,
    ENCIN_STACK_2M = 21,
    ENCIN_STACK_4M = 22,
    ENCIN_STACK_8M = 23,
    ENCIN_STACK_16M = 24,
    ENCIN_STACK_32M = 25,
    ENCIN_STACK_64M = 26,
    ENCIN_STACK_128M = 27,
    ENCIN_STACK_256M = 28,
    ENCIN_STACK_512M = 29,

    ENCIN_STACK_1G = 30,

} encin_stack_size;

char *encin_stack_acquire(encin_stack_size size);

void encin_stack_release(encin_stack_size size, char *stack);

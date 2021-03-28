#include <encin.h>
#include <stdatomic.h>
#include <stdio.h>
#include <unistd.h>

void sigint_handler(struct signalfd_siginfo *info, void *argument) {
    (void)info;

    size_t *press_count_pointer = argument;
    atomic_fetch_add_explicit(press_count_pointer, 1, memory_order_relaxed);
}

int encin(void) {
    size_t press_count = 0;
    encin_signal(SIGINT, sigint_handler, &press_count);

    size_t local_press_count = atomic_load_explicit(&press_count, memory_order_relaxed);
    while (local_press_count < 42) {
        printf("You pressed CTRL+C %zu times...\n", local_press_count);
        usleep(100000);
        local_press_count = atomic_load_explicit(&press_count, memory_order_relaxed);
    }

    return 0;
}

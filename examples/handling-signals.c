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

    while (atomic_load_explicit(&press_count, memory_order_relaxed) < 42) {
        printf(
            "You pressed CTRL+C %zu times...\n",
            atomic_load_explicit(&press_count, memory_order_relaxed)
        );
        usleep(100000);
    }

    return 0;
}

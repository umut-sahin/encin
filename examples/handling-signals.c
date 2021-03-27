#include <encin.h>
#include <stdatomic.h>
#include <stdio.h>
#include <unistd.h>

void sigint_handler(struct signalfd_siginfo *info, void *argument) {
    (void)info;

    size_t *press_count_pointer = argument;
    switch (atomic_fetch_add_explicit(press_count_pointer, 1, memory_order_relaxed)) {
        case 0: {
            printf("You pressed CTRL+C for the first time...\n");
            break;
        }
        case 1: {
            printf("You pressed CTRL+C for the second time...\n");
            break;
        }
        case 2: {
            printf("You pressed CTRL+C for the third time...\n");
            printf("  Fine I'll quit. Sheesh...\n");
            break;
        }
        default: {
            break;
        }
    }
}

int encin(void) {
    size_t press_count = 0;
    encin_signal(SIGINT, sigint_handler, &press_count);

    while (atomic_load_explicit(&press_count, memory_order_relaxed) < 3) {
        usleep(100);
    }

    return 0;
}

#pragma once

#include <encin/stack.h>
#include <signal.h>
#include <sys/signalfd.h>

typedef void (*encin_signal_handler)(struct signalfd_siginfo *info, void *argument);

void encin_signal(int signal, encin_signal_handler handler, void *argument);


#ifndef ENCIN_SIGNAL_LISTENER_STACK_SIZE
#define ENCIN_SIGNAL_LISTENER_STACK_SIZE (ENCIN_STACK_32K)
#endif
static_assert(
    ENCIN_SIGNAL_LISTENER_STACK_SIZE >= ENCIN_STACK_16K
        && ENCIN_SIGNAL_LISTENER_STACK_SIZE <= ENCIN_STACK_4G,
    "ENCIN_SIGNAL_LISTENER_STACK_SIZE is not an instance of encin_stack_size"
);

int encin_signal_start(void);

void encin_signal_stop(void);

#pragma once

#include <signal.h>
#include <sys/signalfd.h>

typedef void (*encin_signal_handler)(struct signalfd_siginfo *info, void *argument);

void encin_signal(int signal, encin_signal_handler handler, void *argument);

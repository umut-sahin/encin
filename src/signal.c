#include <assert.h>
#include <encin.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/signalfd.h>
#include <unistd.h>


static pid_t pid = 0;
static pthread_t tid = 0;
static int fd = 0;

static encin_signal_handler handlers[NSIG] = { NULL };
static void *arguments[NSIG] = { NULL };

static void *signal_listener(void *unused) {
    (void)unused;

    bool stopped = false;
    while (!stopped) {
        struct signalfd_siginfo caught_signal_infos[16];
        ssize_t read_bytes = read(fd, &caught_signal_infos, 16 * sizeof(struct signalfd_siginfo));
        if (read_bytes == -1) {
            continue;
        }

        size_t caught_signal_count = read_bytes / sizeof(struct signalfd_siginfo);
        for (size_t i = 0; i < caught_signal_count; i++) {
            struct signalfd_siginfo *caught_signal_info = &caught_signal_infos[i];

            uint32_t caught_signal_no = caught_signal_info->ssi_signo;
            pid_t caught_signal_sender_pid = caught_signal_info->ssi_pid;

            if (caught_signal_sender_pid == pid && caught_signal_no == SIGUSR2) {
                stopped = true;
                break;
            }

            encin_signal_handler handler
                = atomic_load_explicit(&handlers[caught_signal_no], memory_order_acquire);
            if (handler != NULL) {
                void *argument = arguments[caught_signal_no];
                handler(caught_signal_info, argument);
            }
        }
    }

    return NULL;
}


void encin_signal(int signal, encin_signal_handler handler, void *argument) {
    assert(
        "(encin usage error on encin_set_signal_handler: signal does not exist)"
            && (signal > 0 && signal < NSIG)
    );

    arguments[signal] = argument;
    atomic_store_explicit(&handlers[signal], handler, memory_order_release);
}

int encin_signal_start(void) {
    sigset_t all_signals;
    sigfillset(&all_signals);
    sigprocmask(SIG_BLOCK, &all_signals, NULL);

    pid = getpid();

    fd = signalfd(-1, &all_signals, SFD_CLOEXEC);
    if (fd == -1) {
        sigprocmask(SIG_UNBLOCK, &all_signals, NULL);
        return -1;
    }

    pthread_attr_t signal_listener_attributes;
    int result = pthread_attr_init(&signal_listener_attributes);
    if (result != 0) {
        close(fd);
        sigprocmask(SIG_UNBLOCK, &all_signals, NULL);

        errno = result;
        return -2;
    }

    size_t stack_size = 1 << ENCIN_SIGNAL_LISTENER_STACK_SIZE;
    result = pthread_attr_setstacksize(&signal_listener_attributes, stack_size);
    if (result != 0) {
        pthread_attr_destroy(&signal_listener_attributes);
        close(fd);
        sigprocmask(SIG_UNBLOCK, &all_signals, NULL);

        errno = result;
        return -2;
    }

    result = pthread_create(&tid, &signal_listener_attributes, signal_listener, NULL);
    if (result != 0) {
        pthread_attr_destroy(&signal_listener_attributes);
        close(fd);
        sigprocmask(SIG_UNBLOCK, &all_signals, NULL);

        errno = result;
        return -2;
    }
    pthread_attr_destroy(&signal_listener_attributes);

    return 0;
}

void encin_signal_stop(void) {
    kill(pid, SIGUSR2);
    pthread_join(tid, NULL);

    close(fd);

    sigset_t all_signals;
    sigfillset(&all_signals);
    sigprocmask(SIG_UNBLOCK, &all_signals, NULL);
}

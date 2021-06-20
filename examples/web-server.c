#include <encin.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

const char response[] =

    "HTTP/1.1 200 OK\n"
    "Content-Type: text/plain\n\n"

    "Hello World!\n";

void signals(encin_signal_info *info, void *argument) {
    (void)info;

    bool *terminate_pointer = argument;
    atomic_store_explicit(terminate_pointer, true, memory_order_relaxed);
}

void serve(int fd, size_t *clients) {
    char buffer[8192];

    encin_task reading = encin_read(fd, buffer, 8192, 0);
    encin_await_io(reading);

    encin_task writing = encin_write(fd, response, sizeof(response) - 1, 0);
    encin_await_io(writing);

    close(fd);

    atomic_fetch_sub_explicit(clients, 1, memory_order_relaxed);
}

int encin(void) {
    bool terminate = false;

    encin_signal(SIGINT, signals, &terminate);
    encin_signal(SIGTERM, signals, &terminate);

    int server = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address;
    memset(&address, 0, sizeof(struct sockaddr_in));

    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = INADDR_ANY;

    int result = bind(server, (struct sockaddr *)&address, sizeof(struct sockaddr_in));
    if (result == -1) {
        fprintf(
            stderr,
            "fatal error: unable to bind the server socket to 0.0.0.0:8080 (%s)\n",
            strerror(errno)
        );
        close(server);
        return 1;
    }

    result = listen(server, 128);
    if (result == -1) {
        fprintf(
            stderr,
            "fatal error: unable to start listening for clients (%s)\n",
            strerror(errno)
        );
        close(server);
        return 1;
    }

    printf("info: listening on 0.0.0.0:8080...\n");

    size_t clients = 0;
    while (atomic_load_explicit(&terminate, memory_order_relaxed) == false) {
        encin_task accepting = encin_accept(server, NULL, NULL, 0);
        int client = encin_await_io(accepting);

        atomic_fetch_add_explicit(&clients, 1, memory_order_relaxed);
        encin_spawn_detached(serve, client, &clients, ENCIN_STACK_256K);
    }

    while (atomic_load_explicit(&clients, memory_order_relaxed) > 0) {
        encin_yield();
    }
    close(server);

    return 0;
}

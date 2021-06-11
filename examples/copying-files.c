#include <encin.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int encin(int argc, char *argv[]) {
    if (argc != 3) {
        printf("usage: copying-files <input> <output>\n");
        return 0;
    }

    char *input_path = argv[1];
    char *output_path = argv[2];

    int input_fd = open(input_path, O_RDONLY);
    if (input_fd == -1) {
        fprintf(stderr, "failed to open the input file (%s)\n", strerror(errno));
        return 1;
    }

    int output_fd = open(output_path, O_CREAT | O_WRONLY, 0644);
    if (output_fd == -1) {
        fprintf(stderr, "failed to open the output file (%s)\n", strerror(errno));
        close(input_fd);
        return 1;
    }

    off_t offset = 0;
    size_t chunk = 0;
    while (true) {
        char buffer[8192];

        encin_task reading = encin_read(input_fd, buffer, 8192, offset);
        ssize_t read = encin_await_io(reading);

        if (read == -1) {
            fprintf(
                stderr,
                "failed to read chunk #%zu the input file (%s)\n",
                chunk, strerror(errno)
            );
            close(input_fd);
            close(output_fd);
            return 1;
        }

        encin_task writing = encin_write(output_fd, buffer, read, offset);
        ssize_t written = encin_await_io(writing);

        if (written == -1 || written != read) {
            fprintf(
                stderr,
                "failed to write the chunk #%zu to the output file (%s)\n",
                chunk, strerror(errno)
            );
            close(input_fd);
            close(output_fd);
            return 1;
        }

        if (read < 8192) {
            break;
        } else {
            offset += 8192;
        }
    }

    close(input_fd);
    close(output_fd);
    return 0;
}

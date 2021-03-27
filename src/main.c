#include <encin.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

extern int encin();

int main(int argc, char *argv[], char *envp[]) {
    #define RED(string) "\x1B[31m" string "\x1B[0m"

    int result = encin_signal_start_listener();
    if (result != 0) {
        switch (result) {
            case -1: {
                fprintf(
                    stderr,
                    RED("fatal encin error:")" signal file creation failed (%s)\n",
                    strerror(errno)
                );
                fflush(stderr);
                break;
            }
            case -2: {
                fprintf(
                    stderr,
                    RED("fatal encin error:")" signal listener creation failed (%s)\n",
                    strerror(errno)
                );
                fflush(stderr);
                break;
            }
            default: {
                __builtin_unreachable();
            }
        }
        goto SIGNAL_START_LISTENER_FAILED;
    }

    int code = encin(argc, argv, envp);

    encin_signal_stop_listener();
    return code;

    SIGNAL_START_LISTENER_FAILED:
    exit(EX_OSERR);
}

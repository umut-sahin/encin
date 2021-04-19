#include <encin.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

extern int encin();

int main(int argc, char *argv[], char *envp[]) {
    #define RED(string) "\x1B[31m" string "\x1B[0m"

    int result;

    result = encin_signal_start_listener();
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
        exit(EX_OSERR);
    }

    result = encin_pool_start();
    if (result != 0) {
        switch (result) {
            case -1: {
                fprintf(
                    stderr,
                    RED("fatal encin error:")" encin pool is misconfigured (%s)\n",
                    "ENCIN_POOL_SIZE is not a positive number"
                );
                fflush(stderr);
                break;
            }
            case -2: {
                fprintf(
                    stderr,
                    RED("fatal encin error:")" encin pool is misconfigured (%s)\n",
                    "ENCIN_POOL_SIZE is too large"
                );
                fflush(stderr);
                break;
            }
            case -3: {
                fprintf(
                    stderr,
                    RED("fatal encin error:")" encin job queue creation failed (%s)\n",
                    strerror(errno)
                );
                fflush(stderr);
                break;
            }
            case -4: {
                fprintf(
                    stderr,
                    RED("fatal encin error:")" encin pool creation failed (%s)\n",
                    strerror(errno)
                );
                fflush(stderr);
                break;
            }
            default: {
                __builtin_unreachable();
            }
        }
        encin_signal_stop_listener();
        exit(EX_OSERR);
    }

    result = encin(argc, argv, envp);

    encin_blocking_pool_stop();
    encin_pool_stop();
    encin_signal_stop_listener();

    return result;
}

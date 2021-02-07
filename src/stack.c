#include <assert.h>
#include <encin.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/mman.h>

char *encin_stack_acquire(encin_stack_size size) {
    assert(
        "(encin usage error on encin_stack_acquire: size is not an instance of encin_stack_size)"
        && (size >= ENCIN_STACK_16K && size <= ENCIN_STACK_1G)
    );

    int flags = MAP_PRIVATE | MAP_ANONYMOUS;
    int protection = PROT_READ | PROT_WRITE | PROT_EXEC;
    char *stack = mmap(NULL, 1 << size, protection, flags, -1, 0);
    if (stack == MAP_FAILED) {
        encin_status = ENCIN_STACK_ACQUIRY_FAILURE;
        return NULL;
    }

    return NULL;
}

void encin_stack_release(encin_stack_size size, char *stack) {
    assert(
        "(encin usage error on encin_stack_acquire: size is not an instance of encin_stack_size)"
        && (size >= ENCIN_STACK_16K && size <= ENCIN_STACK_1G)
    );

    if (munmap(stack, 1 << size) == -1) {
        assert(
            "(encin usage error on encin_stack_release: non acquired stack is tried to be released)"
            && false
        );
    }
}

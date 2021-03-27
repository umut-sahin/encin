#include <assert.h>
#include <encin.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/mman.h>

void *encin_stack_acquire(encin_stack_size size) {
    assert(
        "(encin usage error on encin_stack_acquire: size is not an instance of encin_stack_size)"
            && (size >= ENCIN_STACK_8K && size <= ENCIN_STACK_4G)
    );

    int flags = MAP_PRIVATE | MAP_ANONYMOUS;
    int protection = PROT_READ | PROT_WRITE;

    #ifndef NDEBUG
    protection |= PROT_EXEC;
    #endif

    void *stack = mmap(NULL, 1 << size, protection, flags, -1, 0);
    if (stack == MAP_FAILED) {
        encin_status = ENCIN_STACK_ACQUIRY_FAILURE;
        return NULL;
    }

    return stack;
}

void encin_stack_release(encin_stack_size size, void *stack) {
    assert(
        "(encin usage error on encin_stack_acquire: size is not an instance of encin_stack_size)"
            && (size >= ENCIN_STACK_8K && size <= ENCIN_STACK_4G)
    );

    if (munmap(stack, 1 << size) == -1) {
        assert(
            "(encin usage error on encin_stack_release: stack is not acquired with encin_stack_acquire(size))"
                && false
        );
    }
}

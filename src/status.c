#include <assert.h>
#include <encin.h>
#include <stdbool.h>
#include <threads.h>

static thread_local encin_result thread_local_encin_status = ENCIN_SUCCESS;

encin_result *encin_status_pointer() {
    return &thread_local_encin_status;
}

const char *encin_status_description() {
    switch (encin_status) {
        case ENCIN_SUCCESS: {
            return "success";
        }
        case ENCIN_STACK_ACQUIRY_FAILURE: {
            return "stack acquiry failure";
        }
        default: {
            assert(
                "(encin state error on encin_status_description: status is not an instance of encin_result)"
                && false
            );
            __builtin_unreachable();
        }
    }
}

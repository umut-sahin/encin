#include <assert.h>
#include <encin.h>
#include <stdbool.h>

const char *encin_result_description(encin_result result) {
    switch (result) {
        case ENCIN_SUCCESS: {
            return "success";
        }
        case ENCIN_STACK_ACQUIRY_FAILURE: {
            return "stack acquiry failure";
        }
        default: {
            assert(
                "(encin usage error on encin_result_description: result is not an instance of encin_result)"
                    && false
            );

            #ifdef NDEBUG
            __builtin_unreachable();
            #endif
        }
    }
}

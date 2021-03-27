#include <encin.h>
#include <threads.h>

static thread_local encin_result actual_encin_status = ENCIN_SUCCESS;

encin_result *encin_status_pointer() {
    return &actual_encin_status;
}

const char *encin_status_description() {
    return encin_result_description(actual_encin_status);
}

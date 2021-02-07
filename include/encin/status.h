#pragma once

typedef enum {

    ENCIN_SUCCESS = 0,

    ENCIN_STACK_ACQUIRY_FAILURE = -1,

} encin_result;

encin_result *encin_status_pointer();

const char *encin_status_description();

#define encin_status (*encin_status_pointer())

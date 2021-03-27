#pragma once

typedef enum {

    ENCIN_SUCCESS = 0,

    ENCIN_STACK_ACQUIRY_FAILURE = -1,

} encin_result;

const char *encin_result_description(encin_result result);

#pragma once

typedef enum {

    ENCIN_SUCCESS = 0,

    ENCIN_TASK_CREATION_FAILURE = -1,

    ENCIN_STACK_ACQUIRY_FAILURE = -2,

} encin_result;

const char *encin_result_description(encin_result result);

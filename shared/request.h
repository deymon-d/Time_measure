#pragma once

#include "stdlib.h"

typedef enum {
    ADD_EXPERIMENT,
    EXEC_EXPERIMENT
} request_t;

struct Addition {
    size_t count_;
    size_t size_executable_;
    size_t count_tests_;
};

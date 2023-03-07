#pragma once

#include <stdlib.h>
#include <time.h>

struct Experiment {
    char *name_;
    size_t count_;
    char *dir_data_;
    int *returns_results_;
    char *dir_results_;
    time_t *time_results_;
};

struct User {
    struct Experiment *experiments_;
    char *name_;
};

void delete_user(struct User *user);
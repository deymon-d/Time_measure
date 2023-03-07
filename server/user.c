#include "user.h"


void delete_user(struct User *user) {
    free(user->experiments_[0].returns_results_);
    free(user->experiments_[0].time_results_);
    free(user->experiments_);
}
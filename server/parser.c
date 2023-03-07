#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "parser.h"
#include "../shared/copy.h"
#include "../shared/request.h"

int add_experiment(int fd, struct User *user) {
    user->experiments_ = calloc(1, sizeof(*user->experiments_));
    if (!user->experiments_) {
        return 1;
    }
    struct Addition addition = {0};
    if (read(fd, &addition, sizeof(addition)) == -1) {
        return 1;
    }
    struct Experiment *now = user->experiments_;
    now->count_ = addition.count_;
    now->returns_results_ = calloc(now->count_, sizeof(*now->returns_results_));
    if (!now->returns_results_) {
        return 1;
    }
    now->time_results_ = calloc(now->count_, sizeof(*now->time_results_));
    if (!now->time_results_) {
        return 1;
    }
    now->dir_data_ = "data.txt";
    now->dir_results_ = "results.txt";
    now->name_ = "executable_from_client.txt";
    int fd_exec = open(now->name_, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IXUSR);
    if (fd_exec == -1) {
        return 1;
    }
    if (copy(fd, fd_exec, addition.size_executable_)) {
        close(fd_exec);
        return 1;
    }
    close(fd_exec);
    off_t data_size = 0;
    if (read(fd, &data_size, sizeof(data_size)) == -1) {
        return 1;
    }
    int fd_data = open(now->dir_data_, O_CREAT | O_APPEND | O_WRONLY, S_IRUSR | S_IWUSR);
    if (fd_data == -1) {
        return 1;
    }
    if (copy(fd, fd_data, data_size)) {
        close(fd_data);
        return 1;
    }
    close(fd_data);
    return 0;
}

#include <stdio.h>

int exec_experiment(int fd, struct User *user) {
    printf("1\n");
    return 0;
}
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#include "parser.h"
#include "../shared/copy.h"
#include "../shared/request.h"
#include "path.h"

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

enum { BUF_SIZE = 4096, COUNT_NSECS_IN_SEC = 1000000000 };

int exec_experiment(int fd, struct User *user) {
    char buf[BUF_SIZE];
    strcpy(buf, BUILD_DIR);
    size_t pos = strlen(buf);
    for (int i = 0; i < strlen(user->experiments_->name_); ++i) {
        buf[pos + i] = user->experiments_->name_[i];
    }
    for (int i = 0; i < user->experiments_->count_ ; ++i) {
        int fd_input = open(user->experiments_->dir_data_, O_RDONLY);
        if (fd_input == -1) {
            return 1;
        }
        int fd_output = open(user->experiments_->dir_results_, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
        if (fd_output == -1) {
            close(fd_input);
            return 1;
        }
        struct timespec start = {0};
        clock_gettime(CLOCK_REALTIME, &start);
        pid_t pid = fork();
        if (pid == -1) {
            close(fd_input);
            close(fd_output);
            return 1;
        }
        if (!pid) {
            dup2(fd_input, 0);
            dup2(fd_output, 1);
            close(fd_input);
            close(fd_output);
            execlp(buf, user->experiments_->name_, NULL);
            _exit(12);
        }
        int res = 0;
        waitpid(pid, &res, 0);
        user->experiments_->returns_results_[i] = WEXITSTATUS(res);
        struct timespec end = {0};
        clock_gettime(CLOCK_REALTIME, &end);
        user->experiments_->time_results_[i] = (double)(end.tv_sec - start.tv_sec) +
                                               (double)(end.tv_nsec - start.tv_nsec) / COUNT_NSECS_IN_SEC;;
        close(fd_output);
        close(fd_input);
    }
    size_t count = user->experiments_->count_;
    if (write(fd, &count, sizeof(count)) == -1) {
        return 1;
    }
    if (write(fd, user->experiments_->returns_results_, count * sizeof(*user->experiments_->returns_results_)) == -1) {
        return 1;
    }
    if (write(fd, user->experiments_->time_results_, count * sizeof(*user->experiments_->time_results_)) == -1) {
        return 1;
    }
    int fd_ans = open(user->experiments_->dir_results_,  O_RDONLY);
    if (fd_ans == -1) {
        return 1;
    }
    off_t file_size = lseek(fd_ans, 0, SEEK_END);
    if (write(fd, &file_size, sizeof(file_size)) == -1) {
        close(fd_ans);
        return 1;
    }
    lseek(fd_ans, 0, SEEK_SET);
    if (copy(fd_ans, fd, file_size) == -1) {
        close(fd_ans);
        return 1;
    }
    close(fd_ans);
    return 0;
}

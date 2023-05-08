#include "commands.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void work_in_save_mod(struct User * user) {
    printf("1924\n");
    int fd = open("extra_save.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        return;
    }
    int size = user->name_ ? strlen(user->name_) : 0;
    if (write(fd, &size, sizeof(size)) == -1) {
        close(fd);
        return;
    }
    if (write(fd, user->name_, size) == -1) {
        close(fd);
        return;
    }
    printf("1233");
    size = strlen(user->experiments_->name_);
    if (write(fd, &size, sizeof(size)) == -1) {
        close(fd);
        return;
    }
    if (write(fd, user->experiments_->name_, size) == -1) {
        close(fd);
        return;
    }
    size = strlen(user->experiments_->dir_data_);
    if (write(fd, &size, sizeof(size)) == -1) {
        close(fd);
        return;
    }
    if (write(fd, user->experiments_->dir_data_, size) == -1) {
        close(fd);
        return;
    }
    close(fd);
}

void work_in_get_mode(struct User * user) {
    FILE *file = fopen("users_data.txt", "w");
    fprintf(file, "%s:\n", user->name_);
    fprintf(file, "do %u experiments with time results:\n", user->experiments_->count_);
    for (size_t i = 0; i< user->experiments_->count_; ++i) {
        fprintf(file, "%.10gs\n", user->experiments_->time_results_[i]);
    }
    fclose(file);
}

void recovery(struct User *user) {
    int fd = open("extra_save.txt", O_RDONLY);
    if (fd == -1) {
        return;
    }
    int size = 0;
    if (read(fd, &size, sizeof(size)) == -1) {
        close(fd);
        return;
    }
    if (size != 0) {
        user->name_ = calloc(size + 1, sizeof(*user->name_));
        if (!user->name_) {
            close(fd);
            return;
        }
        user->name_[size] = 0;
        if (read(fd, user->name_, size) == -1) {
            close(fd);
            return;
        }
    }

    user->experiments_ = calloc(1, sizeof(*user->experiments_));
    if (!user->experiments_) {
        close(fd);
        return;
    }
    if (read(fd, &size, sizeof(size)) == -1) {
        close(fd);
        return;
    }
    user->experiments_->name_ = calloc(size + 1, sizeof(*user->experiments_->name_));
    if (!user->experiments_->name_) {
        close(fd);
        return;
    }
    user->experiments_->name_[size] = 0;
    if (read(fd, user->experiments_->name_, size + 1) == -1) {
        close(fd);
        return;
    }
    if (read(fd, &size, sizeof(size)) == -1) {
        close(fd);
        return;
    }
    user->experiments_->dir_data_ = calloc(size + 1, sizeof(*user->experiments_->dir_data_));
    if (!user->experiments_->dir_data_) {
        close(fd);
        return;
    }
    user->experiments_->dir_data_[size] = 0;
    if (read(fd, user->experiments_->dir_data_, size + 1) == -1) {
        close(fd);
        return;
    }
    close(fd);
}

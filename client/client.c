#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "../shared/copy.h"
#include "../shared/request.h"


int main(int argc, char *argv[]) {
    if (argc < 4) {
        return 1;
    }
    if (argv[1][0] == 's') {
        int fd = open("channel.txt", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
        if (fd == -1) {
            perror("Can't create request\n");
            return 1;
        }
        request_t request = ADD_EXPERIMENT;
        if (write(fd, &request, sizeof(request)) == -1) {
            perror("Can't create request\n");
            close(fd);
            return 1;
        }
        int fd_exec = open(argv[2], O_RDONLY);
        if (fd_exec == -1) {
            perror("Can't create request\n");
            close(fd);
            return 1;
        }
        off_t file_size = lseek(fd_exec, 0, SEEK_END);
        lseek(fd_exec, 0, SEEK_SET);
        struct Addition addition = {.count_ = 10, .count_tests_ = 1, .size_executable_ = file_size};
        if (write(fd, &addition, sizeof(addition)) == -1) {
            perror("Can't create request\n");
            close(fd);
            close(fd_exec);
            return 1;
        }
        if (copy(fd_exec, fd, -1) == -1) {
            perror("Can't create request\n");
            close(fd);
            close(fd_exec);
            return 1;
        }
        close(fd_exec);
        int fd_test = open(argv[3], O_RDONLY);
        if (fd_test == -1) {
            perror("Can't create request\n");
            close(fd);
            return 1;
        }
        file_size = lseek(fd_test, 0, SEEK_END);
        lseek(fd_exec, 0, SEEK_SET);
        if (write(fd, &file_size, sizeof(file_size)) == -1) {
            perror("Can't create request\n");
            close(fd);
            close(fd_test);
            return 1;
        }
        if (copy(fd_test, fd, -1) == -1) {
            perror("Can't create request\n");
            close(fd);
            close(fd_test);
            return 1;
        }
        close(fd_test);
        request = EXEC_EXPERIMENT;
        if (write(fd, &request, sizeof(request)) == -1) {
            perror("Can't create request\n");
            close(fd);
            return 1;
        }
        close(fd);
        return 0;
    }
}


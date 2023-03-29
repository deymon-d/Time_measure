#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "../shared/copy.h"
#include "../shared/request.h"


int main(int argc, char *argv[]) {
    if (argc < 3) {
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
    } else if (argv[1][0] == 'g') {
        int fd = open(argv[2], O_RDONLY);
        if (fd == -1) {
            perror("don't found server's answer\n");
        }
        size_t count = 0;
        if (read(fd, &count, sizeof(count)) == -1) {
            perror("can't read answer\n");
            return 1;
        }
        printf("%d\n", count);
        double *time_results = calloc(count, sizeof(time_results));
        if (!time_results) {
            close(fd);
            perror("can't read answer\n");
            return 1;
        }
        int *return_results = calloc(count, sizeof(return_results));
        if (!return_results) {
            close(fd);
            free(time_results);
            perror("can't read answer\n");
            return 1;
        }
        if (read(fd, return_results, count * sizeof(*return_results)) == -1) {
            close(fd);
            free(time_results);
            free(return_results);
            perror("can't read answer\n");
            return 1;
        }
        if (read(fd, time_results, count * sizeof(*time_results)) == -1) {
            close(fd);
            free(time_results);
            free(return_results);
            perror("can't read answer\n");
            return 1;
        }
        for (int i = 0; i < count; ++i) {
            printf("Experiment return with code number %d\n"
                   "with time %.10g\n", return_results[i], time_results[i]);
        }
        printf("Experiment returns result:\n");
        off_t file_size = 0;
        if (read(fd, &file_size, sizeof(file_size)) == -1) {
            close(fd);
            free(time_results);
            free(return_results);
            perror("can't read answer\n");
            return 1;
        }
        if (copy(fd, 1, file_size) == -1) {
            close(fd);
            free(time_results);
            free(return_results);
            perror("can't read answer\n");
            return 1;
        }
        close(fd);
        free(time_results);
        free(return_results);
    }
}


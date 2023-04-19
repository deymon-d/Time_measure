#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#include "../shared/request.h"
#include "commands.h"
#include "parser.h"
#include "user.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Not found connection\n");
        return 1;
    }

    struct User user = {0};
    if (!(strcmp(argv[1], "--recovery"))) {
        recovery(&user);
        int fd_ans = open("ans.txt", O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR);
        if (fd_ans == -1) {
            delete_user(&user);
            perror("server dead inside\n");
            return 1;
        }
        if (exec_experiment(fd_ans, &user)) {
            delete_user(&user);
            perror("Bad connection\n");
            close(fd_ans);
            return 1;
        }
        close(fd_ans);
        delete_user(&user);
        return 0;
    }
    prepare_for_work();
    int fd_ans = -1;
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Not found connection\n");
        return 1;
    }
    request_t request = 0;
    ssize_t tmp = 0;
    while ((tmp = read(fd, &request, sizeof(request)))) {
        printf("1\n");
        if (mod != WORK) {
            if (mod == SAVE) {
                work_in_save_mod(&user);
            } else {
                work_in_get_mode(&user);
            }
            mod = WORK;
            continue;
        }
        if (tmp == -1) {
            perror("Bad connection\n");
            close(fd);
            return 1;
        }
        switch (request) {
            case ADD_EXPERIMENT:
                if (add_experiment(fd, &user)) {
                    delete_user(&user);
                    perror("Bad connection\n");
                    close(fd);
                    return 1;
                }
                break;
            case EXEC_EXPERIMENT:
                fd_ans = open("ans.txt", O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR);
                if (fd_ans == -1) {
                    delete_user(&user);
                    perror("server dead inside\n");
                    return 1;
                }
                if (exec_experiment(fd_ans, &user)) {
                    delete_user(&user);
                    perror("Bad connection\n");
                    close(fd_ans);
                    return 1;
                }
                close(fd_ans);
                break;
            default:
                perror("Bad request\n");
                close(fd);
                return 1;
        }
    }
    printf("2\n");
    sleep(1000);
    if (mod != WORK) {
        printf("123\n");
        if (mod == SAVE) {
            printf("1923\n");
            work_in_save_mod(&user);
        } else {
            work_in_get_mode(&user);
        }
        mod = WORK;
    }
    close(fd);
    delete_user(&user);
    return 0;
}


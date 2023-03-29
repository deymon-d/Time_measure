#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


#include "../shared/request.h"
#include "parser.h"
#include "user.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Not found connection\n");
        return 1;
    }
    int fd_ans = -1;
    int fd = open("channel.txt", /*argv[1],*/ O_RDONLY);
    if (fd == -1) {
        perror("Not found connection\n");
        return 1;
    }
    request_t request = 0;
    ssize_t tmp = 0;
    struct User user = {0};
    while ((tmp = read(fd, &request, sizeof(request)))) {
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
    close(fd);
    delete_user(&user);
    return 0;
}


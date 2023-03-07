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
    int fd = open(argv[1], O_RDONLY);
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
                if (exec_experiment(fd, &user)) {
                    perror("Bad connection\n");
                    close(fd);
                    return 1;
                }
                break;
            default:
                perror("Bad request\n");
                close(fd);
                return 1;
        }
    }
    return 0;
}


#include "copy.h"

#include <unistd.h>

int copy(int fd_in, int fd_out, size_t count) {
    ssize_t tmp = 0;
    char val = 0;
    while (count && (tmp = (read(fd_in, &val, sizeof(val))))) {
        if (tmp == -1) {
            return -1;
        }
        if (write(fd_out, &val, sizeof(val)) == -1) {
            return -1;
        }
        --count;
    }
    return 0;
}
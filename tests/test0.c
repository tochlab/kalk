#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "module/kalk.h"

int main(int argc, char **argv) {
    printf("Test0\n");
    int fd = open("/dev/"DEVICE_NAME, O_RDWR);
    if(fd < 0) {
        perror("open");
        return -1;
    }

    int res = ioctl(fd, CMD0, NULL);
    if( res != 0 ) {
        fprintf(stderr,"ioctl error: %d\n", res);
    }

    close(fd);
    return 0;
}
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <string.h>

#include "module/kalk.h"

int test0( void ) {
    printf("Test0 ... ");
    int fd = open("/dev/"DEVICE_NAME, O_RDWR);
    if(fd < 0) {
        perror("open");
        return -1;
    }

    int res = ioctl(fd, CMD0);
    if( res != 0 ) {
        fprintf(stderr,"ioctl error: %d\n", res);
    }

    close(fd);
    printf("OK\n");
    return 0;
}

int test1(char *str) {
    printf("Test1 ... ");
    int fd = open("/dev/"DEVICE_NAME, O_RDWR);
    if(fd < 0) {
        perror("open");
        return -1;
    }
    kalk_args args;
    args.src = (uintptr_t ) str;
    args.dst = 0;
    args.len = strlen(str);

    int res = ioctl(fd, CMD_REVSTR, &args);
    if( res != 0 ) {
        fprintf(stderr,"ioctl error: %d\n", res);
    }
    close(fd);

    printf("result '%s' ", (char *) args.src);
    printf("OK\n");
    return 0;
}

int main(int argc, char **argv) {
    test0();
    const char hello_world[] = "Hello World";
    char *str = strdup(hello_world);
    test1(str);
    free(str);
    return 0;
}
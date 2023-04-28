#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <string.h>

#include "module/kalk.h"

int test0( void ) {
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
    return 0;
}

char *strrev(char *str) {
    size_t len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char x = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = x;
    }
    return str;
}

int test1( void ) {
    char hello_world[] = "Hello World";

    int fd = open("/dev/"DEVICE_NAME, O_RDWR);
    if(fd < 0) {
        perror("open");
        return -1;
    }

    int res = ioctl(fd, CMD_REVSTR, 0);
    if(res == 0) {
        return -1;
    }

    char *str = strdup(hello_world);
    kalk_args args = {0};
    args.src = (uintptr_t ) str;
    args.len = strlen(str);

    res = ioctl(fd, CMD_REVSTR, &args);
    if( res != 0 ) {
        fprintf(stderr,"ioctl error: %d\n", res);
    }
    close(fd);
    res = strcmp(str, strrev(hello_world));
    free(str);
    return res;
}

int main(int argc, char **argv) {
    printf("Test0 ... %s\n", test0() ? "FAIL" : "OK");
    printf("Test1 ... %s\n", test1() ? "FAIL" : "OK");
    return 0;
}
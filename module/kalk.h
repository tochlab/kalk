#ifndef MODULE_KALK_H
#define MODULE_KALK_H

#ifdef __KERNEL__
#include <linux/stddef.h>
#else
#include <stddef.h>
#include <sys/ioctl.h>
#include <stdint.h>
#endif
#define DEVICE_NAME "kalk"

typedef struct {
    uintptr_t src;
    uintptr_t dst;
    size_t len;
} kalk_args;

#define MAGIC 0xAABB

#define CMD0 _IO(MAGIC, 0)
#define CMD_REVSTR _IOWR(MAGIC, 1, kalk_args)

#endif //MODULE_KALK_H

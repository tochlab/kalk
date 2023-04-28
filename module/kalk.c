#include <linux/module.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include "kalk.h"

static int major = 0;
static struct class *cls = NULL;

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static long device_ioctl(struct file *, unsigned int, unsigned long);

static struct file_operations kalk_fops = {
        .open = device_open,
        .release = device_release,
        .unlocked_ioctl = device_ioctl,
};

static int __init kalk_init( void )
{
    major = register_chrdev(0, DEVICE_NAME, &kalk_fops);

    if (major < 0) {
        pr_alert("Registering device failed with %d\n", major);
        return major;
    }

    cls = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);

    pr_info("Created /dev/%s\n", DEVICE_NAME);

    return 0;
}

static void __exit kalk_exit(void) 
{
    device_destroy(cls, MKDEV(major, 0));
    class_destroy(cls);

    /* Unregister the device */
    unregister_chrdev(major, DEVICE_NAME);
}

static int device_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    return 0;
}

long device_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
    int res = -1;
    kalk_args args;
    switch(cmd) {
        case CMD0: {
            pr_info("Ping!\n");
            res = 0;
        }
        break;
        case CMD_REVSTR: {
            int i = 0;
            char *str = NULL;
            size_t len = 0;
            if(copy_from_user(&args, (const void __user *) arg, sizeof(kalk_args))) {
                printk("copy_from_user failed\n");
                return -EINVAL;
            }

            len = args.len;
            str = (char *) kzalloc(len + 1, 0);
            if(!str) {
                return -1;
            }
            if(copy_from_user(str, (void __user *) args.src, len)) {
                pr_err("copy_from_user args.src failed\n");
                kfree(str);
                return -1;
            }
            for(i = 0;i<len/2;i++) {
                char x = str[i];
                str[i] = str[len - i - 1];
                str[len - i - 1] = x;
            }
            if( copy_to_user((void *) args.src, str, len) ) {
                pr_err("copy_to_user args.src failed\n");
                kfree(str);
                return -1;
            }
            res = 0;
        }
        break;
        default:
            pr_info("Unknown IOCTL\n");
    }
    return res;
}

module_init(kalk_init);
module_exit(kalk_exit);

MODULE_LICENSE("GPL");
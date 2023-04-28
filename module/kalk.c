#include <linux/module.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/fs.h>

#define DEVICE_NAME "kalk"

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
        pr_alert("Registering char device failed with %d\n", major);
        return major;
    }

    pr_info("I was assigned major number %d.\n", major);

    cls = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);

    pr_info("Device created on /dev/%s\n", DEVICE_NAME);

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
    return 0;
}

module_init(kalk_init);
module_exit(kalk_exit);

MODULE_LICENSE("GPL");
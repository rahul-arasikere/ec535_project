#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>       /* printk() */
#include <linux/kdev_t.h>       /* initialization */
#include <linux/cdev.h>         /* initialization */
#include <linux/fs.h>           /* everything... */
#include <linux/device.h>
#include <linux/slab.h>         /* kmalloc() */
#include <linux/param.h>
#include <linux/errno.h>        /* error codes */
#include <linux/types.h>        /* size_t */
#include <linux/fcntl.h>        /* O_ACCMODE */
#include <linux/uaccess.h>
#include <linux/pwm.h>          /* pwm api */
#include <asm/uaccess.h>        /* copy_from/to_user */
#include <asm/system_misc.h>    /* cli(), *_flags */

#define ROBOT_MAJOR   61
#define ROBOT_MINOR   0

/* Molude Info */
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Asbel Fontanez (asbelef)");

/* Device Info */
dev_t dev = MKDEV(ROBOT_MAJOR, ROBOT_MINOR); /* Make Device that has the correct MAJOR and MINOR numbers  */
static struct class *dev_class;
static struct cdev robot_cdev;

/* Main module functions */
static int robot_init(void);
static int robot_open(struct inode *inode, struct file *filp);
static int robot_release(struct inode *inode, struct file *filp);
static ssize_t robot_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
static ssize_t robot_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
static void robot_exit(void);

/* Making File Operations List */
static const struct file_operations robot_fops = {
    .owner      = THIS_MODULE,
    .open       = robot_open,
    .read       = robot_read,
    .write      = robot_write,
    .release    = robot_release
};

/* PWM */
struct pwm_device *pwm_left = NULL;
struct pwm_device *pwm_right = NULL;

/* Settinng entry and exit points */
module_init(robot_init);
module_exit(robot_exit);

/* Sets The PWM Signals to a 20ms Period With provide Duty_Cycles in ns */
void set(int left_duty, int right_duty){
    pwm_config(pwm_left, left_duty, 20000000);
    pwm_config(pwm_right, right_duty, 20000000);
    pwm_enable(pwm_left);
    pwm_enable(pwm_right);
}

static int robot_init(void){
    int err;

    err = register_chrdev_region(dev, 1, "robot"); // Request register region

    if(err < 0){
        printk(KERN_ALERT "robot: cannot obtain major number %d\n", ROBOT_MAJOR);
        return err;
    }

    cdev_init(&robot_cdev, &robot_fops); // Assign file operations to the new device

    if((cdev_add(&robot_cdev, dev, 1))){ // Try to add the device
        printk(KERN_ALERT "Cannot add the device to the system\n");
        goto r_class;
    }

    /* Try to create the class */
    if((dev_class = class_create(THIS_MODULE, "robot_class")) == NULL){
        printk(KERN_ALERT "Cannot create the struct class\n");
        goto r_class;
    }

    /* Try to create the device */
    if(device_create(dev_class, NULL, dev, NULL, "robot_device") == NULL){
        printk(KERN_ALERT "Cannot create the device\n");
        goto r_device;
    }
    
    /* Retrieving PWM Devices */
    pwm_left = pwm_request(1, "left-side");
    pwm_right = pwm_request(2, "right-side");

    /* Error Handling */
    if(pwm_left == NULL){
        printk(KERN_ALERT "robot: PWM Left wasn't retrieved");
        goto r_device;
    }

    if(pwm_right == NULL){
        printk(KERN_ALERT "robot: PWM Right wasn't retrieved");
        goto r_device;
    }

    printk(KERN_ALERT "Finished Inserting robot module\n");

    return 0;

    r_device:
        class_destroy(dev_class);
    r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}

static int robot_open(struct inode *inode, struct file *filp){
    return 0;
}

static int robot_release(struct inode *inode, struct file *filp){
    return 0;
}

static ssize_t robot_read(struct file *filp, char *buf, size_t count, loff_t *f_pos){
    return count;
}

static ssize_t robot_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos){
    char *buffer = kmalloc(count, GFP_KERNEL); // Setup input buffer with length count
    char flag = 's';                           // Set the flag to default to stopped position

    /* Copy Informattion From User Space */
    if(copy_from_user(buffer + *f_pos, buf, count)){
        return -EFAULT;
    }
    
    if(buffer){
        flag = buffer[0]; // Set flag to the value of the first character in the buffer
        kfree(buffer);    // Free the memory space taken by the buffer
    }

    /* Flag Validation and Executation */
    if(flag == 'f' || flag == 'F'){
        set(1000000, 1000000);
    }else if(flag == 'b' || flag == 'B'){
        set(2000000, 2000000);
    }else if(flag == 'l' || flag == 'L'){
        set(2000000, 1000000);
    }else if(flag == 'r' || flag == 'R'){
        set(1000000, 2000000);
    }else if(flag == 's' || flag == 'S'){
        pwm_disable(pwm_left);
        pwm_disable(pwm_right);
    }else{
        printk(KERN_ALERT "Robot: Please provide a valid input!");
    }

    return count;
}

/* Disabling and Removing Everything For a Succesfully Removal */
static void robot_exit(void){
    pwm_disable(pwm_left);
    pwm_disable(pwm_right);
    pwm_free(pwm_left);
    pwm_free(pwm_right);

    cdev_del(&robot_cdev);
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    unregister_chrdev(ROBOT_MAJOR, "robot");

    printk(KERN_ALERT "Successfully Removed Module\n");
}
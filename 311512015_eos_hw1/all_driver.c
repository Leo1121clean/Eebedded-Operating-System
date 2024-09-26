/***************************************************************************
**
*   \file
    led_driver.c
*   \details
    Simple GPIO driver explanation
*   \author EmbeTronicX
*   \Tested with Linux raspberrypi 5.4.51-v7l+
******************************************************************************
*/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h> //mdelay()
#include <linux/uaccess.h>  //copy_to/from_user()
#include <linux/gpio.h>     //GPIO

//LED is connected to this GPIO
#define GPIO_18 (18)
#define GPIO_15 (15)
#define GPIO_14 (14)

#define GPIO_21 (21) //A
#define GPIO_20 (20) //B
#define GPIO_16 (16) //C
#define GPIO_12 (12) //D
#define GPIO_1 (1) //E
#define GPIO_7 (7) //F
#define GPIO_8 (8) //G

dev_t dev = 0; //led_device's MAJOR number is 0
static struct class *dev_class;
static struct cdev etx_cdev;

static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);
/*************** Driver functions **********************/
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp,
                char __user *buf, size_t len,loff_t * off);
static ssize_t etx_write(struct file *filp,
                const char *buf, size_t len, loff_t * off);
/******************************************************/

//File operation structure
static struct file_operations fops =
{
    .owner = THIS_MODULE,
    .read = etx_read,
    .write = etx_write,
    .open = etx_open,
    .release = etx_release,
};

/*
** This function will be called when we open the Device file
*/
static int etx_open(struct inode *inode, struct file *file)
{
    pr_info("Device File Opened...!!!\n");
    return 0;
}

/*
** This function will be called when we close the Device file
*/
static int etx_release(struct inode *inode, struct file *file)
{
    pr_info("Device File Closed...!!!\n");
    return 0;
}

/*
** This function will be called when we read the Device file
*/
static ssize_t etx_read(struct file *filp,
                char __user *buf, size_t len, loff_t *off)
{
    uint8_t gpio_state = 0;

    // reading GPIO value
    gpio_state = gpio_get_value(GPIO_18);

    // write to user
    len = 1;
    if( copy_to_user(buf, &gpio_state, len) > 0) {
        pr_err("ERROR: Not all the bytes have been copied to user\n");
    }

    pr_info("Read function : GPIO_18 = %d \n", gpio_state);

    return 0;
}

/*
** This function will be called when we write the Device file
*/
static ssize_t etx_write(struct file *filp,
                    const char __user *buf, size_t len, loff_t *off)
{
    uint8_t rec_buf[9] = {0};

    if( copy_from_user( rec_buf, buf, len ) > 0) {
        pr_err("ERROR: Not all the bytes have been copied from user\n");
    }

    /* led 1 */
    if(rec_buf[0] == '1') //enable
    {
        if(rec_buf[1] == '1') //light on
            gpio_set_value(GPIO_18, 1);
        else if(rec_buf[1] == '0') // light off
            gpio_set_value(GPIO_18, 0);
    }

    /* led 2 */
    if(rec_buf[2] == '1') //enable
    {
        if(rec_buf[3] == '1') //light on
            gpio_set_value(GPIO_15, 1);
        else if(rec_buf[3] == '0') // light off
            gpio_set_value(GPIO_15, 0);
    }

    /* led 3 */
    if(rec_buf[4] == '1') //enable
    {
        if(rec_buf[5] == '1') //light on
            gpio_set_value(GPIO_14, 1);
        else if(rec_buf[5] == '0') // light off
            gpio_set_value(GPIO_14, 0);
    }


    /* seg on */
    if(rec_buf[6] == '1')
    {
        if(rec_buf[7] == '0'){
            gpio_set_value(GPIO_21, 1);
            gpio_set_value(GPIO_20, 1);
            gpio_set_value(GPIO_16, 1);
            gpio_set_value(GPIO_12, 1);
            gpio_set_value(GPIO_1, 1);
            gpio_set_value(GPIO_7, 1);
            gpio_set_value(GPIO_8, 0);
        }
        else if(rec_buf[7] == '1'){
            gpio_set_value(GPIO_21, 0);
            gpio_set_value(GPIO_20, 1);
            gpio_set_value(GPIO_16, 1);
            gpio_set_value(GPIO_12, 0);
            gpio_set_value(GPIO_1, 0);
            gpio_set_value(GPIO_7, 0);
            gpio_set_value(GPIO_8, 0);
        }
        else if(rec_buf[7] == '2'){
            gpio_set_value(GPIO_21, 1);
            gpio_set_value(GPIO_20, 1);
            gpio_set_value(GPIO_16, 0);
            gpio_set_value(GPIO_12, 1);
            gpio_set_value(GPIO_1, 1);
            gpio_set_value(GPIO_7, 0);
            gpio_set_value(GPIO_8, 1);
        }
        else if(rec_buf[7] == '3'){
            gpio_set_value(GPIO_21, 1);
            gpio_set_value(GPIO_20, 1);
            gpio_set_value(GPIO_16, 1);
            gpio_set_value(GPIO_12, 1);
            gpio_set_value(GPIO_1, 0);
            gpio_set_value(GPIO_7, 0);
            gpio_set_value(GPIO_8, 1);
        }
        else if(rec_buf[7] == '4'){
            gpio_set_value(GPIO_21, 0);
            gpio_set_value(GPIO_20, 1);
            gpio_set_value(GPIO_16, 1);
            gpio_set_value(GPIO_12, 0);
            gpio_set_value(GPIO_1, 0);
            gpio_set_value(GPIO_7, 1);
            gpio_set_value(GPIO_8, 1);
        }
        else if(rec_buf[7] == '5'){
            gpio_set_value(GPIO_21, 1);
            gpio_set_value(GPIO_20, 0);
            gpio_set_value(GPIO_16, 1);
            gpio_set_value(GPIO_12, 1);
            gpio_set_value(GPIO_1, 0);
            gpio_set_value(GPIO_7, 1);
            gpio_set_value(GPIO_8, 1);
        }
        else if(rec_buf[7] == '6'){
            gpio_set_value(GPIO_21, 1);
            gpio_set_value(GPIO_20, 0);
            gpio_set_value(GPIO_16, 1);
            gpio_set_value(GPIO_12, 1);
            gpio_set_value(GPIO_1, 1);
            gpio_set_value(GPIO_7, 1);
            gpio_set_value(GPIO_8, 1);
        }
        else if(rec_buf[7] == '7'){
            gpio_set_value(GPIO_21, 1);
            gpio_set_value(GPIO_20, 1);
            gpio_set_value(GPIO_16, 1);
            gpio_set_value(GPIO_12, 0);
            gpio_set_value(GPIO_1, 0);
            gpio_set_value(GPIO_7, 0);
            gpio_set_value(GPIO_8, 0);
        }
        else if(rec_buf[7] == '8'){
            gpio_set_value(GPIO_21, 1);
            gpio_set_value(GPIO_20, 1);
            gpio_set_value(GPIO_16, 1);
            gpio_set_value(GPIO_12, 1);
            gpio_set_value(GPIO_1, 1);
            gpio_set_value(GPIO_7, 1);
            gpio_set_value(GPIO_8, 1);
        }
        else if(rec_buf[7] == '9'){
            gpio_set_value(GPIO_21, 1);
            gpio_set_value(GPIO_20, 1);
            gpio_set_value(GPIO_16, 1);
            gpio_set_value(GPIO_12, 1);
            gpio_set_value(GPIO_1, 0);
            gpio_set_value(GPIO_7, 1);
            gpio_set_value(GPIO_8, 1);
        }
    }
    /* seg off */
    else if(rec_buf[6] == '0')
    {
        gpio_set_value(GPIO_21, 0);
        gpio_set_value(GPIO_20, 0);
        gpio_set_value(GPIO_16, 0);
        gpio_set_value(GPIO_12, 0);
        gpio_set_value(GPIO_1, 0);
        gpio_set_value(GPIO_7, 0);
        gpio_set_value(GPIO_8, 0);
    }

    return len;
}

/*
** Module Init function
*/
static int __init etx_driver_init(void)
{
    /*Allocating Major number*/
    if((alloc_chrdev_region(&dev, 0, 1, "led_Dev")) <0){
        pr_err("Cannot allocate major number\n");
        goto r_unreg;
    }
    pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

    /*Creating cdev structure*/
    cdev_init(&etx_cdev,&fops);

    /*Adding character device to the system*/
    if((cdev_add(&etx_cdev,dev,1)) < 0){
        pr_err("Cannot add the device to the system\n");
        goto r_del;
    }

    /*Creating struct class*/
    if((dev_class = class_create(THIS_MODULE,"led_class")) == NULL){
        pr_err("Cannot create the struct class\n");
        goto r_class;
    }

    /*Creating device*/
    if((device_create(dev_class,NULL,dev,NULL,"led_device")) == NULL){
        pr_err( "Cannot create the Device \n");
        goto r_device;
    }

    //1. Checking the GPIO is valid or not
    if(gpio_is_valid(GPIO_18) == false){
        pr_err("GPIO %d is not valid\n", GPIO_18);
        goto r_device;
    }
    if(gpio_is_valid(GPIO_15) == false){
        pr_err("GPIO %d is not valid\n", GPIO_15);
        goto r_device;
    }
    if(gpio_is_valid(GPIO_14) == false){
        pr_err("GPIO %d is not valid\n", GPIO_14);
        goto r_device;
    }
    if(gpio_is_valid(GPIO_21) == false){
        pr_err("GPIO %d is not valid\n", GPIO_21);
        goto r_device;
    }
    if(gpio_is_valid(GPIO_20) == false){
        pr_err("GPIO %d is not valid\n", GPIO_20);
        goto r_device;
    }
    if(gpio_is_valid(GPIO_16) == false){
        pr_err("GPIO %d is not valid\n", GPIO_16);
        goto r_device;
    }
    if(gpio_is_valid(GPIO_12) == false){
        pr_err("GPIO %d is not valid\n", GPIO_12);
        goto r_device;
    }
    if(gpio_is_valid(GPIO_1) == false){
        pr_err("GPIO %d is not valid\n", GPIO_1);
        goto r_device;
    }
    if(gpio_is_valid(GPIO_7) == false){
        pr_err("GPIO %d is not valid\n", GPIO_7);
        goto r_device;
    }
    if(gpio_is_valid(GPIO_8) == false){
        pr_err("GPIO %d is not valid\n", GPIO_8);
        goto r_device;
    }

    //2. Requesting the GPIO
    if(gpio_request(GPIO_18,"GPIO_18") < 0){
        pr_err("ERROR: GPIO %d request\n", GPIO_18);
        goto r_gpio;
    }
    if(gpio_request(GPIO_15,"GPIO_15") < 0){
        pr_err("ERROR: GPIO %d request\n", GPIO_15);
        goto r_gpio;
    }
    if(gpio_request(GPIO_14,"GPIO_14") < 0){
        pr_err("ERROR: GPIO %d request\n", GPIO_14);
        goto r_gpio;
    }
    if(gpio_request(GPIO_21,"GPIO_21") < 0){
        pr_err("ERROR: GPIO %d request\n", GPIO_21);
        goto r_gpio;
    }
    if(gpio_request(GPIO_20,"GPIO_20") < 0){
        pr_err("ERROR: GPIO %d request\n", GPIO_20);
        goto r_gpio;
    }
    if(gpio_request(GPIO_16,"GPIO_16") < 0){
        pr_err("ERROR: GPIO %d request\n", GPIO_16);
        goto r_gpio;
    }
    if(gpio_request(GPIO_12,"GPIO_12") < 0){
        pr_err("ERROR: GPIO %d request\n", GPIO_12);
        goto r_gpio;
    }
    if(gpio_request(GPIO_1,"GPIO_1") < 0){
        pr_err("ERROR: GPIO %d request\n", GPIO_1);
        goto r_gpio;
    }
    if(gpio_request(GPIO_7,"GPIO_7") < 0){
        pr_err("ERROR: GPIO %d request\n", GPIO_7);
        goto r_gpio;
    }
    if(gpio_request(GPIO_8,"GPIO_8") < 0){
        pr_err("ERROR: GPIO %d request\n", GPIO_8);
        goto r_gpio;
    }

    //3.configure the GPIO as output
    gpio_direction_output(GPIO_18, 0);
    gpio_direction_output(GPIO_15, 0);
    gpio_direction_output(GPIO_14, 0);
    gpio_direction_output(GPIO_21, 0);
    gpio_direction_output(GPIO_20, 0);
    gpio_direction_output(GPIO_16, 0);
    gpio_direction_output(GPIO_12, 0);
    gpio_direction_output(GPIO_1, 0);
    gpio_direction_output(GPIO_7, 0);
    gpio_direction_output(GPIO_8, 0);

    /* Using this call the GPIO 21 will be visible in /sys/class/gpio/
    ** Now you can change the gpio values by using below commands also.
    ** echo 1 > /sys/class/gpio/gpio21/value (turn ON the LED)
    ** echo 0 > /sys/class/gpio/gpio21/value (turn OFF the LED)
    ** cat /sys/class/gpio/gpio21/value (read the value LED)
    **
    ** the second argument prevents the direction from being changed.
    */
    //4. export GPIO
    gpio_export(GPIO_18, false);
    gpio_export(GPIO_15, false);
    gpio_export(GPIO_14, false);
    gpio_export(GPIO_21, false);
    gpio_export(GPIO_20, false);
    gpio_export(GPIO_16, false);
    gpio_export(GPIO_12, false);
    gpio_export(GPIO_1, false);
    gpio_export(GPIO_7, false);
    gpio_export(GPIO_8, false);
    pr_info("Device Driver Insert...Done!!!\n");
    return 0;

r_gpio:
    gpio_free(GPIO_18);
    gpio_free(GPIO_15);
    gpio_free(GPIO_14);
    gpio_free(GPIO_21);
    gpio_free(GPIO_20);
    gpio_free(GPIO_16);
    gpio_free(GPIO_12);
    gpio_free(GPIO_1);
    gpio_free(GPIO_7);
    gpio_free(GPIO_8);
r_device:
    device_destroy(dev_class,dev);
r_class:
    class_destroy(dev_class);
r_del:
    cdev_del(&etx_cdev);
r_unreg:
    unregister_chrdev_region(dev,1);

    return -1;
}

/*
** Module exit function
*/
static void __exit etx_driver_exit(void)
{
    gpio_unexport(GPIO_18);
    gpio_free(GPIO_18);
    gpio_unexport(GPIO_15);
    gpio_free(GPIO_15);
    gpio_unexport(GPIO_14);
    gpio_free(GPIO_14);
    gpio_unexport(GPIO_21);
    gpio_free(GPIO_21);
    gpio_unexport(GPIO_20);
    gpio_free(GPIO_20);
    gpio_unexport(GPIO_16);
    gpio_free(GPIO_16);
    gpio_unexport(GPIO_12);
    gpio_free(GPIO_12);
    gpio_unexport(GPIO_1);
    gpio_free(GPIO_1);
    gpio_unexport(GPIO_7);
    gpio_free(GPIO_7);
    gpio_unexport(GPIO_8);
    gpio_free(GPIO_8);
    device_destroy(dev_class,dev);
    class_destroy(dev_class);
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("Device Driver Remove...Done!!\n");
}
module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX <embetronicx@gmail.com>");
MODULE_DESCRIPTION("A simple device driver - GPIO Driver");
MODULE_VERSION("1.32");
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>                 //kmalloc()
#include <linux/uaccess.h>              //copy_to/from_user()
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");

/* GPIO */

#define RS	26	// 0
#define RW  23	// 1
#define EN  19	// 2
#define D4  13	// 3
#define D5  6	// 4
#define D6	5	// 5
#define D7	11	// 6

#define LINES 2
#define PER_LINE 16

#define mem_size        1024

dev_t dev = 0;
static struct class *dev_class;
static struct cdev pix_cdev;
uint8_t *kernel_buffer;

static void enable_LCD(void);
static void send_4bit_LCD(uint8_t  data);
static void cmd_LCD(uint8_t cmd);
static void init_LCD(void);
static void gotoxy_LCD(uint8_t  x, uint8_t  y);
static void clear_LCD(void);
static void putchar_LCD(uint8_t data);
static void puts_LCD(uint8_t *s);

static void pix_gpio_init(void);
static void pix_gpio_exit(void);

static int pix_file_dev_init(void);
static void pix_file_dev_exit(void);
static int pix_open(struct inode *inode, struct file *file);
static int pix_release(struct inode *inode, struct file *file);
static ssize_t pix_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t pix_write(struct file *filp, const char *buf, size_t len, loff_t * off);

int pix_thread(void *data);
void pix_thread_init(void);
void pix_thread_exit(void);

static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = pix_read,
        .write          = pix_write,
        .open           = pix_open,
        .release        = pix_release,
};

static void enable_LCD()
{
    gpio_set_value(EN, 1);
    //LCD_EN=1;  
    usleep_range(3, 3);
    gpio_set_value(EN, 0);
    //LCD_EN=0;
    usleep_range(50, 50);
}

static void send_4bit_LCD(uint8_t  data)
{
    
    //LCD_D4=(Data>>0)&0x01;
    gpio_set_value(D4, (data & 0b00000001));
    //LCD_D5=(Data>>1)&0x01;
    gpio_set_value(D5, ((data >> 1) & 0b00000001));
    //LCD_D6=(Data>>2)&0x01;
    gpio_set_value(D6, ((data >> 2) & 0b00000001));
    // LCD_D7=(Data>>3)&0x01;
    gpio_set_value(D7, ((data >> 3) & 0b00000001));
}

static void cmd_LCD(uint8_t cmd)
{
    send_4bit_LCD(cmd>>4);
    enable_LCD();
    send_4bit_LCD(cmd);
    enable_LCD();
    usleep_range(2000, 2000);
}

static void init_LCD()
{    
    send_4bit_LCD(0x00);
    usleep_range(20000, 20000);
    gpio_set_value(RS, 0);
    //LCD_RS=0;
    //GPIO_WriteLow(RW_PORT,LCD_RW);
    //LCD_RW=0;
    send_4bit_LCD(0x03);
    enable_LCD();
    usleep_range(5000, 5000);
    enable_LCD();
    usleep_range(100, 100);
    enable_LCD();
    send_4bit_LCD(0x02);
    enable_LCD();
    cmd_LCD( 0x28 );         // giao tiep 4 bit, hien thi 2 hang, ki tu 5x8
    cmd_LCD( 0x0c);          // cho phep hien thi man hinh
    cmd_LCD( 0x06 );         // tang ID, khong dich khung hinh
    clear_LCD();                     // xoa toan bo khung hinh
}

static void gotoxy_LCD(uint8_t  x, uint8_t  y)
{
    uint8_t  pos;
    if(!y)
        pos = (0x80+x);
    else
        pos = (0xC0+x);
    usleep_range(1000, 1000);
    cmd_LCD(pos);
    usleep_range(5000, 5000);
}

static void clear_LCD()
{
    cmd_LCD(0x01);  
    usleep_range(5000, 5000);
}

static void putchar_LCD(uint8_t data)
{
    gpio_set_value(RS, 1);
    //LCD_RS=1;
    cmd_LCD(data);
    gpio_set_value(RS, 0);
    //LCD_RS=0;
}

static void puts_LCD(uint8_t *s)
{
    while (*s)
    {
        putchar_LCD(*s);
        s++;
    }
}

static void pix_gpio_init(void)
{
	printk(KERN_INFO "PIX: starting gpio...");
	gpio_request(RS, "RS");
	gpio_request(RW, "RW");
	gpio_request(EN, "EN");

	gpio_request(D4, "D4");
	gpio_request(D5, "D5");
	gpio_request(D6, "D6");
	gpio_request(D7, "D7");

	gpio_direction_output(RS, 0);
	gpio_direction_output(RW, 0);
	gpio_direction_output(EN, 0);

	gpio_direction_output(D4, 0);
	gpio_direction_output(D5, 0);
	gpio_direction_output(D6, 0);
	gpio_direction_output(D7, 0);

	printk(KERN_INFO "PIX: starting gpio done.");
}

static void pix_gpio_exit(void)
{
	printk(KERN_INFO "PIX: stopping gpio...");
	gpio_free(RS);
	gpio_free(RW);
	gpio_free(EN);

	gpio_free(D4);
	gpio_free(D5);
	gpio_free(D6);
	gpio_free(D7);
	printk(KERN_INFO "PIX: stopping gpio done.");
}



/* file operations */

static int pix_open(struct inode *inode, struct file *file)
{
        /*Creating Physical memory*/
        if((kernel_buffer = kmalloc(mem_size , GFP_KERNEL)) == 0){
            printk(KERN_INFO "Cannot allocate memory in kernel\n");
            return -1;
        }
        clear_LCD();
        printk(KERN_INFO "Device File Opened...!!!\n");
        return 0;
}

static int pix_release(struct inode *inode, struct file *file)
{
		clear_LCD();
		puts_LCD("  Raspberry Pi");
		gotoxy_LCD(5, 1);
		puts_LCD("VinhPN");
        kfree(kernel_buffer);
        printk(KERN_INFO "Device File Closed...!!!\n");
        return 0;
}

static ssize_t pix_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        copy_to_user(buf, kernel_buffer, mem_size);
        printk(KERN_INFO "Data Write : Done!\n");
        return mem_size;
}
static ssize_t pix_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        copy_from_user(kernel_buffer, buf, len);
        clear_LCD();
        puts_LCD(kernel_buffer);
        printk(KERN_INFO "Data Read : Done!\n");
        return len;
}

static int pix_file_dev_init(void)
{
	/*Allocating Major number*/
    if((alloc_chrdev_region(&dev, 0, 1, "pix_Dev")) <0){
            printk(KERN_INFO "Cannot allocate major number\n");
            return -1;
    }
    printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

    /*Creating cdev structure*/
    cdev_init(&pix_cdev,&fops);
    pix_cdev.owner = THIS_MODULE;
    pix_cdev.ops = &fops;

    /*Adding character device to the system*/
    if((cdev_add(&pix_cdev,dev,1)) < 0){
        printk(KERN_INFO "Cannot add the device to the system\n");
        goto r_class;
    }

    /*Creating struct class*/
    if((dev_class = class_create(THIS_MODULE,"pix_class")) == NULL){
        printk(KERN_INFO "Cannot create the struct class\n");
        goto r_class;
    }

    /*Creating device*/
    if((device_create(dev_class,NULL,dev,NULL,"pix_device")) == NULL){
        printk(KERN_INFO "Cannot create the Device 1\n");
        goto r_device;
    }
    printk(KERN_INFO "Device Driver Insert...Done!!!\n");
	return 0;

r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev,1);
    return -1;
}

static void pix_file_dev_exit(void)
{
	device_destroy(dev_class,dev);
    class_destroy(dev_class);
    cdev_del(&pix_cdev);
    unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}


/* THREAD */

#define THREAD_PRIORITY 45
#define THREAD_NAME "pix"

struct task_struct *task;

int pix_thread(void *data)
{
	// struct task_struct *TSK;
	// struct sched_param a;// = { .sched_priority = MAX_RT_PRIO - 50 };
	// //struct sched_param PARAM = { .sched_priority = DEFAULT_PRIO };
	// TSK = current;

	// PARAM.sched_priority = THREAD_PRIORITY;
	// sched_setscheduler(TSK, SCHED_FIFO, &PARAM);

	// while(1) {
	// 	puts_LCD(kernel_buffer);
	// 	usleep_range(30000, 30000);
	// 	if (kthread_should_stop()) break;
	// }
	// return 0;
}

void pix_thread_init(void)
{
	printk(KERN_INFO "PIX: starting thread...");
	task = kthread_run(pix_thread, NULL, THREAD_NAME);
	printk(KERN_INFO "PIX: starting thread done.");
}

void pix_thread_exit(void)
{
	printk(KERN_INFO "PIX: stopping thread...");
	kthread_stop(task);
	printk(KERN_INFO "PIX: stopping thread done.");
}

/* MODULE */

static int __init pix_init(void){
	printk(KERN_INFO "PIX: staring...");
	pix_gpio_init();

	init_LCD();
	kernel_buffer = "  Raspberry Pi";
	puts_LCD(kernel_buffer);
	gotoxy_LCD(5, 1);
	kernel_buffer = "VinhPN";
	puts_LCD(kernel_buffer);

	// pix_thread_init();
	if (pix_file_dev_init() < 0) return -1;
	printk(KERN_INFO "PIX: staring done.");
	return 0;
}

static void __exit pix_exit(void){
	printk(KERN_INFO "PIX: stopping...");

	clear_LCD();
	kernel_buffer = "Shutting ...";
	puts_LCD(kernel_buffer);
	usleep_range(2000, 2000);
	pix_file_dev_exit();
	// pix_thread_exit();
	pix_gpio_exit();
	printk(KERN_INFO "PIX: stopping done.");
}

module_init(pix_init);
module_exit(pix_exit);
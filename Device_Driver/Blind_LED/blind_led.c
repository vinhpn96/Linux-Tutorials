#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <asm/io.h>

#define GPIO_BASE	(0x3F200000)
#define BLOCK_SIZE	(0x100)

#define INP_GPIO_25(b)	*(volatile unsigned int *)(0x00000002 + b) &= (unsigned int)0xFFFC7FFF
#define OUT_GPIO_25(b)	*(volatile unsigned int *)(0x00000002 + b) |= (unsigned int)0x00008000

#define GPIO_SET_25(b)	*(volatile unsigned int *)(0x00000007 + b) |= 0x1000000
#define GPIO_CLR_25(b)	*(volatile unsigned int *)(0x0000000A + b) |= 0x1000000

struct timer_list led_timer;
char kbledstatus = 0;
int status = 0;
unsigned int* addr_base;

static void hello_timer(struct timer_list *ptimer)
{
	printk("jiffies + HZ = %ld + %d\n", jiffies, HZ);
	if (status == 0)
	{
		GPIO_SET_25(addr_base);
		status = 1;
	}
	else {
		GPIO_CLR_25(addr_base);
		status = 0;
	}

	// led_timer.expires = jiffies + HZ;
	// add_timer(&led_timer);
	mod_timer(&led_timer, jiffies + 600);
}

static int hello_init(void)
{
	printk("Hello, LED World!\n");
	addr_base = (unsigned int *)ioremap(GPIO_BASE, BLOCK_SIZE);
	printk("addr_base %X\n", addr_base);
	OUT_GPIO_25(addr_base);
	// init_timer(&led_timer);
	// led_timer.function = hello_timer;
	// led_timer.data = (unsigned long)&kbledstatus;
	// led_timer.expires = jiffies + HZ;
	// add_timer(&led_timer);

	timer_setup(&led_timer, hello_timer, 0);
	mod_timer(&led_timer, jiffies + 600);
	return 0;
}

static void hello_exit(void)
{
	del_timer(&led_timer);
	printk("Goodbye!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TheShellWave");
MODULE_DESCRIPTION("Blind by Module");

#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <asm/io.h>

#define BUTTON 19

static struct timer_list exp_timer;

static void check_button(unsigned long);

static void check_button(unsigned long data)
{
	unsigned int delay = 0.1 * HZ;
	if (gpio_get_value(BUTTON) == 0) {
		printk("vinhpn check_button\n");
	}
	exp_timer.expires = jiffies + delay;
	exp_timer.function = (void *)check_button;
	add_timer(&exp_timer);

}

static int __init check_event_timer_init(void)
{
	printk("BUTTON Polling\n");
	gpio_request(BUTTON, "BUTTON");
	gpio_direction_input(BUTTON);

	exp_timer.expires = jiffies + 1 * HZ;
	exp_timer.function = (void *)check_button;

	add_timer(&exp_timer);
	return 0;
}

static void __exit check_event_timer_exit(void)
{
	printk("END!\n");
	gpio_free(BUTTON);
	del_timer(&exp_timer);
}

module_init(check_event_timer_init);
module_exit(check_event_timer_exit);

MODULE_LICENSE("GPL");
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <asm/io.h>

#define BUTTON 19
#define BASE_ADD_GPIO 0x3F200000
#define SIZE_GPIO 0xA4

unsigned char irq_eint0;
unsigned int *gpio_base;

irqreturn_t external_interrupt_handler(int irq, void *dev_id, struct pt_regs *regs)
{

	// if((gpioa_0_7[4] & (1 << 0)) == 0)
	// {
	// 	if((gpioa_0_7[4] & (1 << 1)) == 0)
	// 	{
	// 		gpioa_0_7[4] |= (1 << 1);
	// 		printk("led_on\n");
	// 	}
	// 	else
	// 	{
	// 		gpioa_0_7[4] &= ~(1 << 1);
	// 		printk("led_off\n");
	// 	}
	// }
	printk("vinhpn interrupt\n");
	return IRQ_HANDLED;

}

static int __init external_interrupt_init(void)
{
	int ret;
	printk("START!\n");
	//Mapping GPIO13
	gpio_base = ioremap(BASE_ADD_GPIO, SIZE_GPIO);
	if(gpio_base == NULL)
	{
		printk("Mapping address gpioa fail\n");
		return -1;
	}

	//Select input mode GPIO19
	gpio_base[1] &= ~(7 << 19);

	//Select output mode GPIO13
	gpio_base[1] &= ~(7 << 9);
	gpio_base[1] |= (1 << 9);

	//GPIO19 Falling Edge
	gpio_base[22] |= (1 << 19);

	//Clear pending GPIO19 bit 
	gpio_base[16] |= (1 << 19);

	//Init interrupt.
	irq_eint0 = gpio_to_irq(BUTTON);
	ret = request_irq(irq_eint0, (irq_handler_t) external_interrupt_handler, IRQF_SHARED, "EINT0", (void *)external_interrupt_handler);
	if(ret)
		printk("Init EINT0 success\n");
	return 0;
}

static void __exit external_interrupt_exit(void)
{
	printk("END!\n");
	iounmap(gpio_base);
	free_irq(irq_eint0, (void *)external_interrupt_handler);
}

module_init(external_interrupt_init);
module_exit(external_interrupt_exit);

MODULE_LICENSE("GPL");
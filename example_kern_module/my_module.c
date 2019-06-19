#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void)
{
	printk( KERN_INFO "Hello, loading example!!!");
}

void cleanup_module(void)
{
	printk( KERN_INFO "Leaving module example!!!");
}

MODULE_LICENSE("GPL");

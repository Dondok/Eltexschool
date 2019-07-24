/*
 * пример создания объекта и файла в /sys для чтения и записи в файл.
 * */
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/parport.h>
#include <asm/uaccess.h>
#include <linux/pci.h>
#include <linux/version.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sysfs.h>

#define LEN_MSG 160
static char *buf;
static struct kobject *test_kobj;
static char buf_msg[ LEN_MSG ] = "Hello from module!\n";

//#define __ATTR_RW(name_file) __ATTR(name, 0644, show, store)
static struct kobj_attribute test1_attr = __ATTR_RW(test1);
/*
static struct kobj_attribute test_test =
	__ATTR(TEST_DONDOK, S_IRUGO | S_IWUSR, test_show,
	       test_store);
*/

/*
 * Метод show() вызывается при чтении файла файловой системы sysfs из простран-
ства пользователя. Он должен скопировать значение атрибута, который передается в ка-
честве параметра attr, в буфер, на который указывает параметр buffer. Размер буфе-
ра равен PAGE_SIZE байт. Для аппаратной платформы x86 значение PAGE_SIZE равно
4096 байтам. Функция должна возвратить количество байтов данных, которые записаны
в буфер при успешном завершении, и отрицательный код в случае возникновения ошибки.
 * */
static ssize_t test1_show(struct kobject *test_kobj,
				 struct kobj_attribute *test_attr, char *buf)
{
	strcpy( buf, buf_msg );
	printk( "read %ld byte\n", strlen( buf ) );
	return strlen( buf );
}
/*
 * Метод store() вызывается при записи. Он должен скопировать size байт данных
из буфера buffer в переменную, представляемую атрибутом attr. Размер буфера все-
гда равен PAGE_SIZE или менее. Функция должна возвратить количество байтов дан-
ных, которые прочитаны из буфера при успешном выполнении, и отрицательный код
ошибки в случае неудачного завершения.
 **/
static ssize_t test1_store(struct kobject *test_kobj,
				  struct kobj_attribute *test_attr,
				  const char *buf, size_t size)
{
	printk( "write %ld byte\n" , size );
	strncpy( buf_msg, buf, size );
	buf_msg[ size ] = '\0';
	return size;
}

static int __init x_init(void)
{
	int result = 0;
	
	test_kobj = kobject_create_and_add("test_kob1", test_kobj);
	if (!test_kobj){
		return -ENOMEM;
	}
		
	result = sysfs_create_file(test_kobj, &test1_attr.attr);
	if (result > 0){
		printk("failed to create file\n")
	}
}
	
static void x_cleanup( void ) {
	
	
	sysfs_remove_file(test_kobj, &test1_attr.attr);
	kobject_del(test_kobj);
}
module_init( x_init );
module_exit( x_cleanup );
MODULE_LICENSE("GPL");

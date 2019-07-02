# 1. После сборки модуля устанавливаем наш модуль в ядро посредством insmod
# "insmod my_cdev".
# 2. Создаем символьный файл в каталоге /dev
# "mknod /dev/name_file" c/b(в зависимости от типа устройства, символьное
# или блочное) major_number minor_number.
# 3. Пытаемся прочитать или записать с файла /dev/name_file  и видим
# результат работы модуля "my_cdev"

obj-m += my_cdev.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean

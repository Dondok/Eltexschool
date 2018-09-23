#компиляция библиотеки
static:
	gcc -c сервер.c -o сервер.o
	ar rc libMY_interface.a сервер.o
	ranlib libMY_сервер.a
#использование библиотеки в программе
main:
	gcc lab5.c libMY_сервер.a

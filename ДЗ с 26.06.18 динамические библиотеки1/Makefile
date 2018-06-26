#Makefile for calc
# сначала компилируются цели libcalc.so
a.out: libcalc.so
	gcc main.c -o a.out -lcalc

	
#В Linux практически все библиотеки имеют префикс lib.
#gcc-shared создает файл общего объекта для общей библиотеки.
#Tак же как и в a.out сначала выполняются цели sum.o dif.o div.o mult.o
#После чего выходной файл будет libcalc.so(динамическая библиотека)-
#нужно переместить в папку (/lib) или (/usr/lib)
libcalc.so: sum.o dif.o div.o mult.o
	 gcc -shared sum.o dif.o div.o mult.o -o libcalc.so


#Cоздание объектных файлов для динамической библиотеки.
#Флаги   -fPIC   и   -fpic  разрешают  генерацию-
#"позиционно-независимого    кода"    (position    independent   code),-
#требующегося   для   разделяемых   библиотек.
sum.o:
	gcc -fPIC -c sum.c
dif.o:
	gcc -fPIC -c dif.c
div.o:
	gcc -fPIC -c div.c
mult.o:
	gcc -fPIC -c mult.c

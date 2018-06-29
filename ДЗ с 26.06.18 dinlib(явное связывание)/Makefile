#Makefile for calc
# сначала компилируются цели libsum.so libdif.so libdiv.so libmult.so
a.out: libsum.so libdif.so libdiv.so libmult.so
	gcc -Wall main.c  -lsum -ldif -ldiv -lmult -ldl -o a.out
#	gcc 2.c -L. -l1d -ldl -o 2

	
#В Linux практически все библиотеки имеют префикс lib.
#gcc-shared создает файл общего объекта для общей библиотеки.
#Tак же как и в a.out сначала выполняются цели sum.o dif.o div.o mult.o
#После чего выходной файл будет lib<name>.so(динамическая библиотека)-
#нужно переместить в папку (/lib) или (/usr/lib).
#или же отредактировать файл /etc/ld.so.conf(добавить туда текущее
#местоположение библиотеки) и сделать ldconfig.
libsum.so: sum.o
	 gcc -shared sum.o -o libsum.so
libdif.so: dif.o
	 gcc -shared dif.o -o libdif.so
libdiv.so: dif.o
	 gcc -shared div.o -o libdiv.so
libmult.so: dif.o
	 gcc -shared mult.o -o libmult.so
	 

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

a.out: libfm.so
	gcc main.c -o a.out -lncurses -lfm
#переместить библиотеку в папку (/lib) или (/usr/lib)
libfm.so: func.o
	gcc -shared func.o -o libfm.so

func.o:
	gcc -fPIC -c func.c

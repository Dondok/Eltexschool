main.c: libpowers.so
	gcc main.c -o main -ldl
libpowers.so: lib.o
	gcc -shared lib.o -o libpowers.so

lib.o:
	gcc -fPIC -c lib.c



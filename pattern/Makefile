# Makefile for World project

binary: client.o libinterface.a
	gcc -o binary client.o -L. -linterface

client.o: client.c
	gcc -c client.c

libworld.a: server.o server2.o
	ar cr libworld.a server.o server2.o

server.o: server.c
	gcc -c server.c

server2.o: server2.c
	gcc -c server2.c

clean:
	rm -f *.o *.a binary

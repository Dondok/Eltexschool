# Makefile for World project

#Опция -L указывает линковщику, где ему искать библиотеку.
#В случае, если библиотека располагается в каталоге /lib или /usr/lib
#то вопрос отпадает сам собой и опция -L не требуется.
#-L. точка означает текущий каталог
# сначала компилируются цели main.o libworld.a
binary: main.o libworld.a
	gcc -o binary main.o -L. -libworld.a 

#создает объектный файл main.o
main.o: main.c
	gcc -c main.c
	
	
#команда ar создает статическую библиотеку (архив). В нашем случае два 
#объектных файла объединяются в один файл libworld.a.
#В Linux практически все библиотеки имеют префикс lib.

#так же как и в binary сначала цели h_world.o g_world.o
libworld.a: h_world.o g_world.o
	ar cr libworld.a h_world.o g_world.o

#создает объектный файл h_world.o
h_world.o: h_world.c
	gcc -c h_world.c

#создает объектный файл g_world.o
g_world.o: g_world.c
	gcc -c g_world.c

clean:
	rm -f *.o *.a binary

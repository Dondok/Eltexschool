#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>




int main()
{
	
	int fd[2];
	pid_t pid;
	char *buf = "hello world\n";

	if (pipe(fd) < 0)
		printf("ошибка вызова функции pipe");
	if ((pid = fork()) < 0) {
		printf("ошибка вызова функции fork");
	} else if (pid > 0) {  //родительский процесс 
		close(fd[0]); //закрываем дескриптор на чтение
		write (fd[1], &buf, sizeof(buf));
	} else if(pid == 0) {
		 //дочерний процесс 

		close(fd[1]);  //закрываем дескриптор на запись
		read (fd[0], buf, sizeof(buf));
		printf ("%s\n", buf);
	}
	
	exit(0);
}

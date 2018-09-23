//создать конвейер по примеру - ls -la|cut -b 1-20
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{	
	int fd[2], pipefd[2];
	
	pid_t child;
	
	pipe(fd);
	pipe (pipefd);
	child = fork();
	if(child != 0) {
		close (fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		execl("/bin/ls","ls","-la",NULL);
    } else if(child == 0){
		close(fd[1]);
		dup2( fd[0], STDIN_FILENO);
		execl("/usr/bin/cut","cut","-b","1-20",NULL); 
	}
	return 0;
}

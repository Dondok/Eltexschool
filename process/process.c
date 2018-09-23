/*	задание: написать программу который создает дочерний процесс,
 * 	вывести PID дочернего процесса и процесса родителя
 */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{	pid_t child_pid, parent_pid;
	
	fprintf(stdout, "PID parent programm %i\n",parent_pid=(int) getppid());
	fprintf(stdout, "PID programm %i\n",parent_pid=(int) getpid());
	printf("\n");
	child_pid = fork();
	
	sleep (child_pid != 0);
	if (child_pid == 0){
		fprintf(stdout, "child_PID = %i\n", child_pid=(int) getpid());
		fprintf(stdout, "parent_PID = %i\n", child_pid=(int) getppid());
	}


		
	return 0;
}

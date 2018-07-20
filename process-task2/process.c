/*	Задание:	
 *						PID, PPID
 * 					/				\
 *			PID, PPID				PID, PPID
 * 		/			\					|		
 * PID, PPID	PID, PPID			PID, PPID		
 * 
 * 
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main(){
	int i,k;
	pid_t A,B,D;
	
	
	fprintf(stdout, "PID parent programm %i\n",	 getppid());
	fprintf(stdout, "PID programm %i\n",	 getpid());
	printf("\n");
	
	for (i = 1; i <= 2; i++){
		A = fork();
		if (A == 0){
			fprintf(stdout, "F%i PPID %i\n",i,	 A =(int)getppid());
			fprintf(stdout, "F%i PID %i\n\n",i, A =(int)getpid());
			if (i == 1) 
			{
				for (k = 1; k <= 2; k++){
					B = fork();
					if (B == 0){
						fprintf(stdout, "F1.%i PPID %i\n", k,B =(int)getppid());
						fprintf(stdout, "F1.%i PID %i\n\n", k,B =(int)getpid());
						break;
					}		
				}
					
			}
			if (i == 2){
				D = fork ();
				if (D == 0){
					fprintf(stdout, "F2.1 PPID %i\n",B =(int)getppid());
					fprintf(stdout, "F2.1 PID %i\n\n" ,B =(int)getpid());
				}
			}
			break;
		}
	} while(wait(0)>0);
	
	return 0;
}		

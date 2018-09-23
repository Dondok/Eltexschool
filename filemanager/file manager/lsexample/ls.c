#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char* argv[]){
fprintf(stdout, "Before child process creating: PARENT ID = %i\n",
(int) getpid());
//if(execvp(argv[1], argv)==-1)
//perror("execvp call : ");
execvp("ls", argv);
fprintf(stdout, "Everything is ignored!\n");
return 0;
}

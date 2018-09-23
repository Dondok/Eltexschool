#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <unistd.h>

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>





//значение x=149	
int dir_list( struct dirent **ent, char ldir[255])
{
	DIR *dir;
	int i=0;
		dir = opendir(ldir);
		while ((ent[i] = readdir(dir)) != NULL)
		{
		printf("first: %s\n", ent[i]->d_name);
		i++;
		}

	return 0;
}

int main(int argc, char ** argv)
{
	
	char    		rdir[255],ldir[255];

	//DIR *dir	;
	
	struct dirent *ent;
	//получаем полный путь тек-го каталога
	getcwd(rdir,255);
    getcwd(ldir,255);
    
dir_list(&ent, ldir);
	

	
return 0;
}

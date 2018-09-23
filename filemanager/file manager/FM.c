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



void sig_winch(int signo)
{
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
	resizeterm(size.ws_row, size.ws_col);
}

int list_dir(struct dirent **ent, WINDOW * left, char ldir[255])
{
	DIR *dir;
	int i=0;
	dir=opendir(ldir);
	while ((ent[i] = readdir(dir)) != NULL)
		{
			if(ent[i]->d_type == DT_DIR)
			wprintw(left,"DIR: %s\n", ent[i]->d_name);
			else
			wprintw(left,"file:%s\n", ent[i]->d_name);
			wrefresh(left);
		}	
		wrefresh(left);
		closedir(dir);
}


int open_dir(struct dirent **ent, WINDOW * left, char ldir[255], dir_number)
{
	DIR *dir;
	int i=0;
	dir=opendir(ldir);
	while ((ent[i] = readdir(dir)) != dir_number)
		{
			if(ent[i]->d_type == DT_DIR)
			wprintw(left,"DIR: %s\n", ent[i]->d_name);
			else
			wprintw(left,"file:%s\n", ent[i]->d_name);
			wrefresh(left);
		}	
		wrefresh(left);
		closedir(dir);
	
	
}

int main(int argc, char ** argv)
{
	WINDOW * right;
	WINDOW * left;
	char    		rdir[255],ldir[255], buf[255];
	
	int MAX_DIR=255;
	int i=0,y=0, max, dir_nimber;
	struct dirent **dirent;
	struct dirent *ent;
	//
	int  left_namelist[255];
    int  right_namelist[255];
	//получаем полный путь тек-го каталога
	getcwd(rdir,255);
    getcwd(ldir,255);
    
	initscr();
	signal(SIGWINCH, sig_winch);
	cbreak();
	curs_set(TRUE);
	refresh();
	
	/*размер окна и начало расположения окна*/
	left = newwin(40, 148/2, 0, 0);
	right = newwin(40, 148/2, 0,148/2+1);
	
	//вывод содержимого текущих каталогов
	list_dir(&ent,left,ldir);
	


	delwin(left);
	delwin(right);
	refresh();
	getch();
	
	endwin();
	exit(EXIT_SUCCESS);
}

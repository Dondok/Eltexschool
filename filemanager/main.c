/*
 * написать файловый менеджер с двумя окнами
 * с возможностью входа в нужную директорию
 */
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

#include "for_fm.h"

	

int main()
{
	/*правое и левое окно, win - вспомогательное окно, будет хранить
	один из окон*/
	WINDOW * left;
	WINDOW * right;
	WINDOW * win;
	//будет хранить количесто файлов + папок возвращаемых от scandir
	int ln, rn;

	int y = 0; //для бегунка 
	int cycle = 0; //для while
	
	// параметры будут хранить текущить путь директории
	char   ldir[255],  rdir[255];
	
	//для хранения списка файлов и каталогов
	struct dirent **lnamelist;
	struct dirent **rnamelist;
	
	keypad(stdscr, TRUE);
	noecho();
	initscr();
	
	cbreak();
	curs_set(FALSE);
	refresh();
	start_color();
	
	//получаем текущий путь относительно расположения данного файла
    getcwd(ldir,255);
    getcwd(rdir,255);
    
    //задаем размеры окон
	left = newwin(60, 148/2, 0, 0);
	right = newwin(60, 148/2, 0,148/2+1);
	
/* вызавается функция list_dir с выводом на экране всех элементов в текущей
 * директории и бегунком указывающий на "выбранный" элемент.
 * дальше получаем нажатие клавиш. Клавиши 'l'- left и 'r'- right
 * указывает переход в нужное окно(по умолчанию left)
 */
	win = left;
    while (cycle == 0)
    { 	
		ln = scandir(ldir, &lnamelist, NULL, alphasort);
		rn = scandir(rdir, &rnamelist, NULL, alphasort);
		wclear(win);
		
		
		if (win == left) {
			list_dir(lnamelist, ln, y, win);
		}
		if (win == right) {
			list_dir(rnamelist, rn, y, win);
		}
		        
		switch ( getch() )
		{
			case 'w':
			case KEY_UP:
				if (y > 0) y--;
				break;
				
			case 's':
			case KEY_DOWN:
				if (win == left){
					if (y < --ln) y++;
				}
				if (win == right){
					if (y < --rn) y++;
				}
				break;
					
			case 'e':
			case KEY_ENTER:
				if (win == right) {
					strcpy(rdir,rnamelist[y] -> d_name);
					chdir(rdir);
					getcwd(rdir,255);
				y = 0;
				}
				if (win == left){
					strcpy(ldir,lnamelist[y] -> d_name);
					chdir(ldir);
					getcwd(ldir,255);
				y = 0;
				}
				
				break;
				 
			case 'l':
				win = left;
				wrefresh(win);
				break;
			
			case 'r':
				win = right;
				wrefresh(win);
				break;	
				
			case 'q':
				cycle = 1;
		}
		noecho();
		wrefresh(win);
			
	}
	
	delwin(left);
	delwin(right);
	delwin(win);
	refresh();
	endwin();
	exit(EXIT_SUCCESS);
}

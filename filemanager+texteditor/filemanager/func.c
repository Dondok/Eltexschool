#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

void list_dir(struct dirent **namelist, int ln, int y, WINDOW *win)
{
	for (int i = 0 ;i < ln; i++){
		if (i == y){
			waddch (win,'>');
		} else {      
			waddch (win,' ');
        }
             
		if (namelist[i] -> d_type == DT_DIR) {
			if (i == y) {
				wattron (win,A_BOLD);
				wprintw (win," DIR: %s\n", namelist[i] -> d_name);
				wattroff (win,A_BOLD);
				wrefresh (win);
			} else {
				wprintw (win," DIR: %s\n", namelist[i] -> d_name);
				wrefresh (win);
			}
		} else {
			if (i == y) {
				wattron (win,A_BOLD);
				wprintw (win," file: %s\n", namelist[i] -> d_name);
				wattroff (win,A_BOLD);
			} else {
				wprintw (win," file: %s\n", namelist[i] -> d_name);
			}
				wrefresh (win);
		}
	}
}

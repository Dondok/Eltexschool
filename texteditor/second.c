#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <curses.h>
void sig_winch(int signo)
{
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
	resizeterm(size.ws_row, size.ws_col);
}

//для границ окон
int limit_x=149, limit_y=34;
/* Глобальное значение текущего положения курсора 0,0 */
int y, x;

//функция управление клавишами
int keyboard ();

//функция для записи на экран
int input ();

int len (int lineno);

int main(int argc, char ** argv)
{
	int y = 0,x = 0;
	int line=0, i=0, n;
	FILE *file;
	char symbol;
	
	initscr();
	signal(SIGWINCH, sig_winch);
	//отображение курсора
	curs_set(TRUE);	
	//включение клавиши
	keypad (stdscr, TRUE);
	refresh();
	standout ();
	mvaddstr (limit_y + 2, limit_x / 2, "PRESS");
	refresh();
	mvaddstr (limit_y + 3, limit_x / 3, "w-save");
	mvaddstr (limit_y + 3, limit_x / 2, "q-quit");
	refresh();
	standend ();
	move(0,0);
	
	//считывание с файла символов и вывод на экран сиволов
	file = fopen ("file.txt","r");
	i=0;
	while(fread(&symbol, sizeof(char), 1, file)>0){
			addch(symbol);	
	}
	fclose (file);
	refresh ();
	move (0, 0);
	//функция для управления клавишами
	keyboard();
	
		//запись в файл
	file = fopen ("file.txt", "w");
	for ( int y = 0; y < limit_y; y++) {
		n = len(y);
		for (x = 0; x < n; x++)
		//A_CHARTEXT	Выделяет один символ.
		putc (mvinch (y, x) & A_CHARTEXT, file);
		putc('\n', file);
	}
	fclose(file);
	
	refresh();
	getch();
	endwin();
	exit(EXIT_SUCCESS);
}

int len (int lineno)
		{
		int linelen = limit_y;
		while (linelen >= 0 && mvinch (lineno, linelen) == ' ')
		linelen --;
		return linelen + 1;
		}
		

/* Режим ввода: принимает и вставляет символы Выход: CTRL+D
incsh Вставка символа перемещение остальные строки,
включая символ в текущей позиции
x++ перемещение по строке*/


int input ()
{
	int c;
	refresh ();
	mvaddstr (limit_y + 1, limit_x / 2, "Режим ввода");
	refresh ();
	move(y,x);		
	for (;;){
		c = getch ();
		if (c == CTRL	('D')){
		break;}
		
		x++;
		if(x == limit_x){
			y++;
			x=0;
		}
		
		insch (c);
		move (y, x);
		refresh ();
	}
	move (limit_y + 1, limit_x / 2);
	clrtoeol ();
	move (y, x);
	refresh ();
}



int keyboard()
{
	int c;
	for(;;)
	{
		move(y,x);
		refresh();
		/*читаем символ с клавиатуры и записываем в "с"*/
		c=getch();
		/* Команды редактора */
		switch(c)
		{
			/*стрелки перемещают курсор в указанном направлении */
			case KEY_LEFT:
			if (x > 0) x--;
			break;
		
		
			case KEY_RIGHT:
			if (x < limit_x) x++;
			break;
			
			
			case KEY_DOWN:
			if (y < limit_y) y++;
			break;

					
			case KEY_UP:
			if (y > 0) y--;
			break;
			/*конец движения курсора*/
			
			
			
			/* i: переход в режим ввода */
			case 'i':
			input ();
			break;
			
			
			/* удалить текущий символ "Delete" */
			case KEY_DC:
			delch ();
			break;
			/* d: удалить текущую строку */
			case 'd':
			deleteln ();
			break;
			
			
			/* w: записать и закончить работу */
			case 'w':
			return 0;
			
			
			/* q: закончить работу без записи файла */
			case 'q':
			endwin ();
			exit (2);
			
			
			default:
			flash ();
			break;
		}
		
		
	}
}

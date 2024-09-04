#include <ncurses.h>

#include "game.h"



int main()
{
	initscr();
	raw();
	
	noecho();

  	game();

	endwin();
	return 0;
}

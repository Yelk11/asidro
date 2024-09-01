#include <ncurses.h>
#include "map.h"
#include "entity.h"




void initializeGame()
{
    genMap();
}

void updateGame()
{
    int ch;
    position start_pos;
    start_pos.x = 5;
    start_pos.y = 5;
    entity* player = createEntity(start_pos, '@');

    while(getch() != 'q'){
        ch = getch();
        moveEntity(player, ch);
        updateMap();
        refresh();
	}
    
}

void drawGame()
{

}



void freeGame()
{
    freeMap();
}


void game()
{
    initializeGame();
    updateGame();
    freeGame();
}


#include <ncurses.h>
#include "map.h"
#include "entity.h"
#include "game.h"
#include "screen.h"


void initializeGame(game_t* game)
{
    game->map = genMap();
}

void updateGame(game_t* game)
{
    int ch;
    position_t start_pos;
    start_pos.x = 5;
    start_pos.y = 5;
    entity_t* player = createEntity(start_pos, '@');
    nodelay(stdscr, true);
    while(getch() != 'q'){
        ch = getch();
        moveEntity(player, ch);
        updateMap(game->map);
        update_screen(game->map);
        
	}
    
}

void freeGame(game_t* game)
{
    freeMap(game->map);
}


void game(void)
{
    game_t* game = calloc(1, sizeof(game_t));
    initializeGame(game);
    updateGame(game);
    freeGame(game);
}


#include <ncurses.h>
#include <stdlib.h>

#include "map.h"
#include "entity.h"
#include "game.h"
#include "screen.h"
#include "linked_list.h"

void initializeGame(game_t* game)
{
    game->map = genMap();
    position_t* start_pos = create_position(5,5);
    entity_t* player = createEntity(start_pos, '@');
    add_entity_map(player);
}

void updateGame(game_t* game)
{
    int ch;
    int pX = 5;
    int pY = 5;
    curs_set(0);
    nodelay(stdscr, true);
    updateMap(game->map);
    update_screen(game->map);
    do{
        ch = getch();
        
        
	}while(ch != 'q');
    
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


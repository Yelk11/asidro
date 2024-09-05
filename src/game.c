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
    game->map->player = createEntity(start_pos, '@');
    
}

void updateGame(game_t* game)
{
    int ch;

    curs_set(0);
    nodelay(stdscr, true);
    
    do{
        ch = getch();
        moveEntity(game->map->player, ch);
        updateMap(game->map);
        update_screen(game->map);
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


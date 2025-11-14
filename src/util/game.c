#include <ncurses.h>
#include <stdlib.h>

#include "map.h"
#include "entity.h"
#include "game.h"
#include "screen.h"
#include "linked_list.h"

void initializeGame(game_t* game)
{
    game->map = map_init();
    position_t* start_pos = create_position(5,5);
    game->player = createEntity(start_pos, '@');
    
}

void updateGame(game_t* game)
{
    int ch;

    curs_set(0);
    nodelay(stdscr, true);
    
    do{
        ch = getch();
        moveEntity(game->player, ch);
        update_screen(game);
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


#include <ncurses.h>
#include <stdlib.h>

#include "map.h"
#include "actor.h"
#include "game.h"
#include "screen.h"
#include "sched.h"
#include "item.h"

void initializeGame(game_t* game)
{
    int seed = 0;
    game->map = map_init(seed);
    game->map->level=1;
    map_gen(game->map);
    game->ch = '0';

    make_actor('@', 0,0,10, player_act, game);

    game->action_list = sched_init(game->player);
}

void updateGame(game_t* game)
{
    curs_set(0);
    nodelay(stdscr, true);
    
    do{
        game->ch = getch();
        sched_advance(game->action_list);
        update_screen(game);
	}while(game->ch != 'q');
    
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


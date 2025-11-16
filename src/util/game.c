#include <ncurses.h>
#include <stdlib.h>

#include "game.h"
#include "game_cfg.h"
#include "map.h"
#include "actor.h"
#include "screen.h"
#include "sched.h"
#include "item.h"

void game_init_level(game_t* game)
{
    map_gen(game->map);
    game_spawn_player(game);
    game_spawn_npc(game);
    game_spawn_monsters(game);
}

void initializeGame(game_t* game)
{
    int seed = 0;
    game->map = map_init(seed);
    game->map->level=1;
    game->ch = '0';
    game_init_level(game);
}

void updateGame(game_t* game)
{
    curs_set(0);
    nodelay(stdscr, true);
    
    do{
        game->ch = getch();
        if(game->ch == 'p')
        {
            game_debug_dump(game);
        }
        sched_cycle_actions(game->action_list);
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


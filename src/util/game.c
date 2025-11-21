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
    nodelay(stdscr, false);  /* Wait for player input */
    
    do{
        /* Check if player is dead */
        actor_t* player = sched_get_player(game->action_list);
        if (!player || !player->isAlive) {
            /* Draw game over screen and wait for Q */
            update_screen(game);
            do {
                game->ch = getch();
            } while(game->ch != 'q');
            break;
        }
        
        /* Wait for player input */
        game->ch = getch();
        if(game->ch == 'p')
        {
            game_debug_dump(game);
            continue;
        }
        
        if (player && player->act) {
            player->act(player);
        }

        sched_node* cur = game->action_list;
        if (cur) {
            do {
                sched_node* next = cur->next;
                
                if (cur->entity && cur->entity->type != PLAYER && cur->entity->act) {
                    cur->entity->act(cur->entity);
                }
                
                cur = next;
            } while (cur != game->action_list);
        }
        
        /* Remove dead actors */
        game->action_list = sched_remove_dead(game->action_list);
        
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


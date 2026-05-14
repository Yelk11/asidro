

#include <ncurses.h>
#include <stdlib.h>

#include "game.h"
#include "ecs.h"
#include "game_cfg.h"
#include "map.h"
#include "sched.h"
#include "screen.h"

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
    ecs_init();
    game->map = map_init(seed);
    game->map->level = 1;
    game->ch = '0';
    game_init_level(game);
}

void updateGame(game_t* game)
{
    curs_set(0);
    nodelay(stdscr, false);

    do {
        Entity player = sched_get_player(game->action_list);
        if (player == ECS_INVALID_ENTITY || actor_is_dead(player)) {
            update_screen(game);
            do {
                game->ch = getch();
            } while (game->ch != 'q');
            break;
        }

        game->ch = getch();
        if (game->ch == 'p') {
            game_debug_dump(game);
            continue;
        }

        game->action_list = sched_cycle_actions(game->action_list, game);
        game->action_list = sched_remove_dead(game->action_list);

        update_screen(game);
    } while (game->ch != 'q');
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
    free(game);
}

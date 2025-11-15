#include <stdlib.h>

#include "game.h"
#include "map.h"

void game_spawn_player(game_t* game)
{
    int x;
    int y;
    map_get_player_spawn(game->map, &x, &y);
    game->action_list = sched_init(make_actor(PLAYER, x,y,10, player_act, game));
}

void game_spawn_monsters(game_t* game)
{

}

void game_spawn_npc(game_t* game)
{

}
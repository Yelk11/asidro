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
    int x;
    int y;
    actor_t* p = sched_get_player(game->action_list);
    for(int i = 0; i < 50; i++)
    {
        map_get_npc_spawn(game->map, &x, &y, p->x, p->y);
        sched_add(game->action_list, make_actor(MONSTER,x,y,10,monster_act,game));
    }
}

void game_spawn_npc(game_t* game)
{

}
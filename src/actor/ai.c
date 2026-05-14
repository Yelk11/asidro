#include "ai.h"
#include <stdlib.h>
#include "map.h"
#include "sched.h"

int distance_to_player(Entity player, Entity actor)
{
    Position* player_pos = ecs_get_position(player);
    Position* actor_pos = ecs_get_position(actor);

    if (!player_pos || !actor_pos) return -1;

    int dx = player_pos->x - actor_pos->x;
    int dy = player_pos->y - actor_pos->y;
    return abs(dx) + abs(dy);
}

void wander_randomly(Entity actor, game_t* game)
{
    if (!ecs_is_valid(actor) || !game) return;

    Position* pos = ecs_get_position(actor);
    if (!pos || !game->map) return;

    const int dirs[4][2] = {{0,-1},{0,1},{-1,0},{1,0}};
    int tries = 6;
    for (int t = 0; t < tries; t++) {
        int i = rand() % 4;
        int nx = pos->x + dirs[i][0];
        int ny = pos->y + dirs[i][1];

        if (!map_is_walkable(game->map, nx, ny)) continue;
        Entity other = sched_get_actor_by_coords(game->action_list, nx, ny);
        if (other != ECS_INVALID_ENTITY && !actor_is_dead(other)) continue;

        pos->x = nx;
        pos->y = ny;
        return;
    }
}

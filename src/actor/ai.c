/**
 * @file ai.c
 * @brief Artificial Intelligence algorithms for NPC/monster behavior
 * @author Asidro Team
 * 
 * Implements distance calculations, pathfinding, and movement algorithms
 * for non-player characters and monsters.
 */

#include "ai.h"
#include <stdlib.h>
#include "map.h"
#include "sched.h"
#include "game.h"


/**
 * @brief calculates the distance to the player
 * @param player the player
 * @param actor the non player actor
 */
int distance_to_player(actor_t* player, actor_t* actor)
{
	if (!player || !actor) return -1;
	int dx = player->x - actor->x;
	int dy = player->y - actor->y;
	/* Manhattan distance is cheap and fine for AI decisions */
	return abs(dx) + abs(dy);
}


/**
 * @brief algorythm for the actor to wander randomly
 * @param actor the actor to walk randomly
 */
void wander_randomly(actor_t* actor)
{
	if (!actor) return;

	/* Actor->data is expected to point to game_t so we can access the map */
	game_t* game = (game_t*)actor->data;
	if (!game || !game->map) {
		return;
	}

	/* Randomly try a few directions and move to the first valid one */
	const int dirs[4][2] = {{0,-1},{0,1},{-1,0},{1,0}}; /* N,S,W,E */
	int tries = 6;
	for (int t = 0; t < tries; t++) {
		int i = rand() % 4;
		int nx = actor->x + dirs[i][0];
		int ny = actor->y + dirs[i][1];

		/* Check map bounds and walkability */
		if (!map_is_walkable(game->map, nx, ny)) continue;

		/* Ensure no other actor occupies the target tile */
		actor_t* other = sched_get_actor_by_coords(game->action_list, nx, ny);
		if (other && other->isAlive) continue;

		/* Move actor */
		actor->x = nx;
		actor->y = ny;
		return;
	}
	/* If no valid move found, stay in place */
}


/**
 * @file ai.h
 * @brief Artificial Intelligence algorithms for entity behavior
 * @author Asidro Team
 * 
 * Provides AI functions for calculating distances, pathfinding,
 * and random movement behavior for NPCs and monsters.
 */

#ifndef AI_H
#define AI_H

#include "actor.h"

/**
 * @brief Calculate Manhattan distance to player
 * 
 * Computes the Manhattan distance (taxicab distance) between an actor
 * and the player. Distance = |dx| + |dy|.
 * 
 * @param player Player actor to measure distance to
 * @param actor Actor to measure distance from
 * 
 * @return Manhattan distance between actors, or INT_MAX if player is NULL
 * 
 * @note Used for deciding monster behavior (chase, attack, wander)
 * @see monster_act
 */
int distance_to_player(actor_t* player, actor_t* actor);

/**
 * @brief Move actor to a random adjacent walkable tile
 * 
 * Selects a random adjacent tile that is walkable and not occupied
 * by other actors. Tries up to 10 times before giving up.
 * 
 * @param actor Actor to move
 * 
 * @note Modifies actor->x and actor->y in place
 * @note Does nothing if no valid adjacent tiles exist
 * @see map_is_walkable, sched_get_actor_by_coords
 */
void wander_randomly(actor_t* actor);

#endif
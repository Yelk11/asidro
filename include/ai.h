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

#include "ecs.h"
#include "game.h"

int distance_to_player(Entity player, Entity actor);
void wander_randomly(Entity actor, game_t* game);

#endif
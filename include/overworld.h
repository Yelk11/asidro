/**
 * @file overworld.h
 * @brief Overworld terrain generation
 * @author Asidro Team
 * 
 * Provides functions for procedurally generating overworld terrain
 * using Perlin-like noise algorithms.
 */

#ifndef OVERWORLD_H
#define OVERWORLD_H

#include "map.h"

/**
 * @brief Generate Perlin-like overworld terrain
 * 
 * Fills the map with procedurally generated overworld terrain including
 * dirt (FLOOR), trees (TREE), rocks (ROCK), and soil (SOIL) tiles.
 * 
 * Uses value noise with fractional Brownian motion (fBm) for natural
 * looking terrain variation.
 * 
 * @param map Map to populate with terrain
 * 
 * @note Map must be already allocated before calling
 * @see map_init
 */
void generate_overworld(map_t *map);

/**
 * @brief Get player spawn point on overworld
 * 
 * Finds a suitable starting location for the player on the overworld.
 * 
 * @param map Overworld map
 * @param x Output X coordinate
 * @param y Output Y coordinate
 * 
 * @see over_get_npc_spawn, map_get_player_spawn
 */
void over_get_player_spawn(map_t* map, int* x, int* y);

/**
 * @brief Get NPC spawn point on overworld away from player
 * 
 * Finds a suitable spawn location for NPCs/monsters on the overworld
 * that is away from the player position.
 * 
 * @param map Overworld map
 * @param x Output X coordinate
 * @param y Output Y coordinate
 * @param px Player X coordinate (to distance from)
 * @param py Player Y coordinate (to distance from)
 * 
 * @see over_get_player_spawn, map_get_npc_spawn
 */
void over_get_npc_spawn(map_t* map, int* x, int* y, int px, int py);

#endif

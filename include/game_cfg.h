/**
 * @file game_cfg.h
 * @brief Game configuration and actor spawning
 * @author Asidro Team
 * 
 * Provides functions for spawning game entities and accessing configuration
 * parameters during game initialization and runtime.
 */

#ifndef GAME_CFG_H
#define GAME_CFG_H

#include "game.h"

/**
 * @brief Spawn the player actor
 * 
 * Creates the player actor at the designated spawn point and initializes
 * the game scheduler with the player as the starting actor.
 * 
 * @param game Game state
 * 
 * @note Must be called before other spawn functions
 * @see game_spawn_monsters, game_spawn_npc
 */
void game_spawn_player(game_t* game);

/**
 * @brief Spawn monster actors
 * 
 * Creates a group of monster entities (50 by default) at random locations
 * away from the player. Monsters are added to the existing scheduler.
 * 
 * @param game Game state
 * 
 * @note game_spawn_player() must be called first
 * @see game_spawn_player, game_spawn_npc
 */
void game_spawn_monsters(game_t* game);

/**
 * @brief Spawn NPC actors
 * 
 * Creates a group of NPC entities (50 by default) at random locations
 * away from the player. Currently use monster AI by default.
 * 
 * @param game Game state
 * 
 * @note game_spawn_player() must be called first
 * @see game_spawn_player, game_spawn_monsters
 */
void game_spawn_npc(game_t* game);

/**
 * @brief Write debug information to files
 * 
 * Dumps the current game state to debug files for analysis:
 * - debug_map.txt: ASCII representation of the map
 * - debug_actor_list.txt: Scheduler ring dump with all actors
 * 
 * Called when player presses 'p' during gameplay.
 * 
 * @param game Game state to dump
 * 
 * @note Files are created in current working directory
 */
void game_debug_dump(game_t* game);

#endif
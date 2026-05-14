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


void game_spawn_player(game_t* game);

void game_spawn_monsters(game_t* game);

void game_spawn_npc(game_t* game);

void game_debug_dump(game_t* game);

#endif
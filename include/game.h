/**
 * @file game.h
 * @brief Core game state and management
 * @author Asidro Team
 * 
 * Defines the main game state structure and provides functions for
 * game initialization, updates, and cleanup.
 */

/*
 * This file defines the game_t structure and function prototypes
 * for game management.
 */
#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "actor.h"
#include "sched.h"

typedef struct bsp_node bsp_node;

/**
 * @struct game_t
 * @brief Main game state container
 * 
 * Holds all global game state including the current map,
 * actor scheduler, and input information.
 */
typedef struct game_t {
	map_t *map;                 /**< Current map/level */
    sched_node* action_list;    /**< Scheduler ring of all actors */
    char ch;                    /**< Last input character */
    bsp_node* root;             /**< BSP tree root (for dungeon generation) */
} game_t;


/**
 * @brief Initialize a new game level
 * 
 * Generates map terrain and spawns player, NPCs, and monsters.
 * 
 * @param game Game state to initialize
 * 
 * @see game_spawn_player, game_spawn_npc, game_spawn_monsters
 */
void initializeGame(game_t* game);

/**
 * @brief Main game loop
 * 
 * Processes input, executes actor turns, updates UI, and continues
 * until player quits (presses 'q') or dies.
 * 
 * @param game Game state to update
 * 
 * @note Blocking function - runs until game ends
 * @see player_act, monster_act
 */
void updateGame(game_t* game);

/**
 * @brief Clean up game state
 * 
 * Deallocates all game resources including map and actors.
 * 
 * @param game Game state to free
 * 
 * @note Should be called before program exit
 * @see freeMap
 */
void freeGame(game_t* game);

/**
 * @brief Game entry point
 * 
 * Allocates game state, runs initialization, main loop, and cleanup.
 * High-level function that orchestrates the entire game lifecycle.
 * 
 * @note No parameters - uses ncurses stdscr directly
 * 
 * @see initializeGame, updateGame, freeGame
 */
void game(void);

/**
 * @brief Check if a tile is a valid spawn location
 * 
 * Determines if an actor can spawn at a specific location.
 * 
 * @param map Map to check
 * @param x X coordinate
 * @param y Y coordinate
 * 
 * @return true if location is valid for spawning, false otherwise
 * 
 * @see place_actor_at
 */
bool valid_spawn_tile(map_t* map, int x, int y);

/**
 * @brief Place an actor at a specific location
 * 
 * Attempts to position an actor at the given coordinates.
 * 
 * @param game Game state
 * @param npc Actor to place
 * @param x Target X coordinate
 * @param y Target Y coordinate
 * 
 * @return true if placement succeeded, false otherwise
 * 
 * @see valid_spawn_tile
 */
bool place_actor_at(game_t* game, actor_t* npc, int x, int y);

/**
 * @brief Place NPCs using BSP tree for better distribution
 * 
 * Distributes NPCs throughout the map using Binary Space Partition tree.
 * 
 * @param game Game state
 * @param root BSP tree root node
 * 
 * @see place_npcs_open_map
 */
void place_npcs_bsp(game_t* game, bsp_node* root);

/**
 * @brief Place NPCs randomly on open map
 * 
 * Randomly distributes NPCs across the entire map.
 * 
 * @param game Game state
 * @param count Number of NPCs to place
 * 
 * @see place_npcs_bsp
 */
void place_npcs_open_map(game_t* game, int count);

#endif // GAME_H

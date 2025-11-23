/**
 * @file map.h
 * @brief Game map representation and utilities
 * @author Asidro Team
 * 
 * Defines the map structure, tile constants, and functions for map
 * generation, manipulation, and pathfinding queries.
 */

#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

#include "underworld.h"

/**
 * @def MAP_HEIGHT
 * @brief Map height in tiles (80 tiles)
 */
#define MAP_HEIGHT 80

/**
 * @def MAP_WIDTH
 * @brief Map width in tiles (80 tiles)
 */
#define MAP_WIDTH 80

/**
 * @defgroup TileTypes Tile Type Constants
 * @brief ASCII characters representing different tile types
 * @{
 */
#define WALL '#'    /**< Impassable wall */
#define FLOOR '.'   /**< Walkable floor */
#define TREE 'T'    /**< Tree (overworld) */
#define SOIL '~'    /**< Soil/water (overworld) */
#define ROCK 'o'    /**< Rock (overworld) */
/** @} */

/**
 * @struct map_t
 * @brief Represents a game map/level
 * 
 * Stores the 2D grid of tiles, metadata, and structural information
 * for BSP-based dungeon generation.
 */
typedef struct map_t{
    char map[MAP_HEIGHT][MAP_WIDTH]; /**< 2D array of tile characters */
    int level;                       /**< Current dungeon level (1-based) */
    int seed;                        /**< Random seed used for generation */
    bsp_node* root;                  /**< BSP tree root (for dungeons) */
} map_t;

/**
 * @brief Create and initialize a new map
 * 
 * Allocates and generates a new map using procedural generation.
 * 
 * @param seed Random seed for terrain generation
 * 
 * @return Pointer to newly allocated map_t, or NULL on failure
 * 
 * @note Caller is responsible for freeing with freeMap()
 * @see map_gen, freeMap
 */
map_t* map_init(int seed);

/**
 * @brief Generate terrain for a map
 * 
 * Fills the map array with procedurally generated terrain based on
 * the map's seed and generator algorithm.
 * 
 * @param map Map to generate terrain for
 * 
 * @see map_init
 */
void map_gen(map_t* map);

/**
 * @brief Free all map resources
 * 
 * Deallocates the map structure and any associated data.
 * 
 * @param map Map to free
 * 
 * @note Should be called before program exit
 * @see map_init
 */
void freeMap(map_t* map);

/**
 * @brief Check if coordinates are within map bounds
 * 
 * @param x X coordinate
 * @param y Y coordinate
 * 
 * @return 1 if in bounds, 0 otherwise
 */
int in_bounds(int x, int y);

/**
 * @brief Set a tile to a specific character
 * 
 * @param map Map to modify
 * @param x X coordinate
 * @param y Y coordinate
 * @param c Character to set
 */
void set_tile(map_t* map, int x, int y, char c);

/**
 * @brief Get the character at a map tile
 * 
 * @param map Map to query
 * @param x X coordinate
 * @param y Y coordinate
 * 
 * @return Character at tile, or '#' (WALL) if out of bounds
 */
char get_tile(map_t* map, int x, int y);

/**
 * @brief Fill entire map with a character
 * 
 * Sets every tile on the map to the same character.
 * Useful for initialization or clearing.
 * 
 * @param map Map to fill
 * @param c Character to fill with
 */
void fill_map(map_t *map, char c);

/**
 * @brief Move to level above (ascend stairs)
 * 
 * Generates the previous dungeon level. Currently a stub.
 * 
 * @param map Map to update
 */
void map_ascend(map_t* map);

/**
 * @brief Move to level below (descend stairs)
 * 
 * Generates the next dungeon level. Currently a stub.
 * 
 * @param map Map to update
 */
void map_descend(map_t* map);

/**
 * @brief Get starting position for player
 * 
 * Finds a suitable spawn location for the player character.
 * 
 * @param map Map to search
 * @param x Output X coordinate
 * @param y Output Y coordinate
 * 
 * @see map_get_npc_spawn
 */
void map_get_player_spawn(map_t* map, int* x, int* y);

/**
 * @brief Check if a tile is walkable
 * 
 * Determines if an actor can move through a tile.
 * Returns false for walls and out-of-bounds positions.
 * 
 * @param map Map to check
 * @param x X coordinate
 * @param y Y coordinate
 * 
 * @return true if walkable, false otherwise
 */
bool map_is_walkable(map_t* map, int x, int y);

/**
 * @brief Get spawn position for NPC/monster away from player
 * 
 * Finds a suitable spawn location that is away from the player.
 * 
 * @param map Map to search
 * @param x Output X coordinate
 * @param y Output Y coordinate
 * @param px Player X coordinate (to avoid)
 * @param py Player Y coordinate (to avoid)
 * 
 * @see map_get_player_spawn
 */
void map_get_npc_spawn(map_t* map, int *x, int* y, int px, int py);

#endif
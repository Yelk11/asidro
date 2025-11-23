#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include "map.h"

#include <time.h>
#include "overworld.h"
#include "underworld.h"


/**
 * @brief initialises the map
 * @param seed the seed to generate the map from
 */
map_t *map_init(int seed)
{
    map_t *map = calloc(1, sizeof(map_t));
    map->level = 0;
    map->seed = seed;
    return map;
}

/**
 * @brief generate the map
 * @param map the map to be generated
 */
void map_gen(map_t* map)
{
    if(map->level == 0)
    {
        generate_overworld(map);
        
    }else{
        map->root = generate_underworld(map);
        
    }
}

/**
 * @brief 
 */
void map_ascend(map_t* map)
{
    /* overworld starts at 0, dungeon 1-infinity */
    if(map->level >= 1)
    {
        map->level--;
    }
    map_gen(map);
}

void map_descend(map_t* map)
{
    if(map->level >= 1)
    {
        map->level++;
    }
    map_gen(map);
}



void freeMap(map_t *map)
{
    free(map);
}

int in_bounds(int x, int y)
{
    return x >= 0 && y >= 0 && x < MAP_WIDTH && y < MAP_HEIGHT;
}

void set_tile(map_t *map, int x, int y, char c)
{
    if (!map)
        return;
    if (in_bounds(x, y))
        map->map[y][x] = c;
}

char get_tile(map_t *map, int x, int y)
{
    if (!map)
        return WALL;
    return in_bounds(x, y) ? map->map[y][x] : WALL;
}

void fill_map(map_t *map, char c)
{
    if (!map)
        return;
    for (int y = 0; y < MAP_HEIGHT; y++)
        for (int x = 0; x < MAP_WIDTH; x++)
            map->map[y][x] = c;
}

void map_get_player_spawn(map_t* map, int* x, int* y)
{
    if(map->level == 0)
    {
        over_get_player_spawn(map, x, y);
    }
    else
    {
        under_get_player_spawn(map, x, y);
    }
}

bool map_is_walkable(map_t* map, int x, int y)
{
    char ch = map->map[y][x];
    return (ch == FLOOR) || (ch == SOIL);
}
void map_get_npc_spawn(map_t* map, int *x, int* y, int px, int py)
{
    if(map->level == 0)
    {
        over_get_npc_spawn(map, x, y, px, py);
    }
    else
    {
        under_get_npc_spawn(map, x, y, px, py);
    }
}
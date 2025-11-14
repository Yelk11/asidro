#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "map.h"

#include <time.h>
#include "overworld.h"
#include "underworld.h"

/* Minimal map implementation used by the game runtime. Previously there were
 * multiple map implementations during experimentation; provide a simple
 * implementation that delegates to the overworld generator by default.
 */

map_t *map_init(int seed)
{
    map_t *map = calloc(1, sizeof(map_t));
    map->level = 0;
    map->seed = seed;
    return map;
}

void map_gen(map_t* map)
{
    if(map->level == 0)
    {
        generate_overworld(map);
    }else{
        generate_underworld(map);
    }
}

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

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

map_t *map_init(void)
{
    return map_gen(0, 1);
}

map_t* map_gen(int level, int seed)
{
    map_t *map = calloc(1, sizeof(map_t));
    if (!map) return NULL;
    if(level == 0)
    {
        generate_overworld(map, seed);
    }else{
        generate_underworld(map, level, seed);
    }
    return map;
}

map_t* ascend(map_t* map)
{

}

map_t* descend(map_t* map)
{

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

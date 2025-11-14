#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "map.h"

#include <time.h>

/* Minimal map implementation used by the game runtime. Previously there were
 * multiple map implementations during experimentation; provide a simple
 * implementation that delegates to the overworld generator by default.
 */

map_t *map_create(void)
{
    /* default: create an overworld with a time-based seed */
    extern map_t *map_create_overworld(int seed);
    int seed = (int)time(NULL);
    return map_create_overworld(seed);
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

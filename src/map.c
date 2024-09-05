#include <stdlib.h>

#include "map.h"
#include "linked_list.h"



map_t* genMap(void)
{
    
    map_t* map = calloc(1, sizeof(map_t));
    for(int x = 0; x < MAP_WIDTH; x++)
    {
        for(int y = 0; y < MAP_HEIGHT; y++)
        {
            map->map[y][x] = '.';
        }
    }
    return map;
}

void freeMap(map_t* map)
{
    free(map);
}


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "map.h"
#include "linked_list.h"



map_t* map_create(void)
{
    
    map_t* map = calloc(1, sizeof(map_t));
    /*
        init map with all walls
    */

    srand(time(NULL));
    fill_map(map, WALL);




    return map;
}

void freeMap(map_t* map)
{
    free(map);
}

int in_bounds(int x, int y) {
    return x >= 0 && y >= 0 && x < MAP_WIDTH && y < MAP_HEIGHT;
}

void set_tile(map_t* map, int x, int y, char c) {
    if (in_bounds(x, y))
        map->map[y][x] = c;
}

char get_tile(map_t* map, int x, int y) {
    return in_bounds(x, y) ? map->map[y][x] : WALL;
}

// --- Map Initialization ---

void fill_map(map_t *map, char c) 
{
    for (int y = 0; y < MAP_HEIGHT; y++)
        for (int x = 0; x < MAP_WIDTH; x++)
            map->map[y][x] = c;
}

// --- Tunnel Algorithm ---

void carve_corridor(map_t* map, Point start, Point end, int width) {

}

// --- Room Placement Example ---

void carve_room(map_t *map, Point center, int w, int h) {
    for (int y = -h/2; y <= h/2; y++)
        for (int x = -w/2; x <= w/2; x++)
            set_tile(map, center.x + x, center.y + y, FLOOR);
}







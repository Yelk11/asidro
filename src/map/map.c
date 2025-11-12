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

    srand(0);
    fill_map(map, WALL);
    // carve_corridor(map, 0, 20, 2, 10, 0.5, EA, 300);
    map_carve(map, 50, 20, 10, 10);



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

void map_carve(map_t* map, int x1, int y1, int x2, int y2)
{
    /* Support any ordering of coordinates: normalize to min/max */
    int sx = (x1 < x2) ? x1 : x2;
    int ex = (x1 > x2) ? x1 : x2;
    int sy = (y1 < y2) ? y1 : y2;
    int ey = (y1 > y2) ? y1 : y2;

    /* Clamp to map bounds */
    if (sx < 0) sx = 0;
    if (sy < 0) sy = 0;
    if (ex >= MAP_WIDTH) ex = MAP_WIDTH - 1;
    if (ey >= MAP_HEIGHT) ey = MAP_HEIGHT - 1;

    for (int x = sx; x <= ex; x++) {
        for (int y = sy; y <= ey; y++) {
            set_tile(map, x, y, FLOOR);
        }
    }
}
// --- Tunnel Algorithm ---
/*
fill map with walls

designate start(left) and end(right)

Crawler
    corridorWidth -same val for each generation
    stepLength -same val for each generation
    changeDirProb



*/
void carve_corridor(map_t* map, int x, int y, int corridorWidth, int stepLength, double changeDirProb, int dir, int gen) 
{
    if(gen <= 0)
    {
        return;
    }
    int newX = x;
    int newY = y;
    for(int step = 0; step < stepLength; step++)
    {
        
        for(int width = -corridorWidth / 2; width < corridorWidth; width++)
        {
            switch(dir)
            {
                case NO:
                    set_tile(map, x + width, y - step, ' ');
                    newX = x;
                    newY = y + step;
                    break;
                case SO:
                    set_tile(map, x + width, y + step, ' ');
                    newX = x;
                    newY = y - step;
                    break;
                case EA:
                    set_tile(map, x + step, y + width, ' ');
                    newX = x + step;
                    newY = y;
                    break;
                case WE:
                    set_tile(map, x - step, y + width, ' ');
                    newX = x - step;
                    newY = y;
                    break;
            }
        }
    }
    int newDir = 0;

    do {
        newDir = rand() % 4 + 1;
    } while (newDir == opposite_direction(dir));

    carve_corridor(map, newX, newY, corridorWidth, stepLength, changeDirProb, newDir, --gen);
}
int opposite_direction(int dir)
{
    switch(dir)
    {
        case NO:
            return SO;
        case SO:
            return NO;
        case EA:
            return WE;
        case WE:
            return EA;
        default:
            return 0;
    }
}
// --- Room Placement Example ---

void carve_room(map_t *map, Point center, int w, int h) {
    for (int y = -h/2; y <= h/2; y++)
        for (int x = -w/2; x <= w/2; x++)
            set_tile(map, center.x + x, center.y + y, FLOOR);
}







#ifndef MAP_H
#define MAP_H

#include "linked_list.h"
#include "entity.h"

#define MAP_HEIGHT 25
#define MAP_WIDTH 80

#define WALL '#'
#define FLOOR '.'

typedef struct map_t{
    char map[MAP_HEIGHT][MAP_WIDTH];
}map_t;


typedef struct {
    int x, y;
} Point;



map_t* map_create(void);

void freeMap(map_t* map);

int in_bounds(int x, int y);

void set_tile(map_t* map, int x, int y, char c);

char get_tile(map_t *map, int x, int y);

void fill_map(map_t *map, char c);

void carve_corridor(map_t* map, Point start, Point end, int width);

void carve_room(map_t *map, Point center, int w, int h);

#endif
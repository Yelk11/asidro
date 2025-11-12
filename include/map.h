#ifndef MAP_H
#define MAP_H

#include "linked_list.h"
#include "entity.h"

#define MAP_HEIGHT 1000
#define MAP_WIDTH 1000

#define WALL '#'
#define FLOOR '.'

/* DIRECTION */
#define NO 0
#define SO 1
#define EA 2
#define WE 3

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

void carve_corridor(map_t* map, int x, int y, int corridorWidth, int stepLength, double changeDirProb, int dir, int gen);

void carve_room(map_t *map, Point center, int w, int h);

int opposite_direction(int dir);

void map_carve(map_t* map, int x1, int y1, int x2, int y2);

#endif
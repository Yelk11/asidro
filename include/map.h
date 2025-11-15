#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

#include "underworld.h"

#define MAP_HEIGHT 100
#define MAP_WIDTH 100

#define WALL '#'
#define FLOOR '.'
#define TREE 'T'
#define SOIL '~'
#define ROCK 'o'

typedef struct map_t{
    char map[MAP_HEIGHT][MAP_WIDTH];
    int level;
    int seed;
    bsp_node* root;
} map_t;

map_t* map_init(int seed);
void map_gen(map_t* map);
void freeMap(map_t* map);
int in_bounds(int x, int y);
void set_tile(map_t* map, int x, int y, char c);
char get_tile(map_t* map, int x, int y);
void fill_map(map_t *map, char c);
void map_ascend(map_t* map);
void map_descend(map_t* map);
void map_get_player_spawn(map_t* map, int* x, int* y);
bool map_is_walkable(map_t* map, int x, int y);
#endif
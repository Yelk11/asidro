#ifndef MAP_H
#define MAP_H



#define MAP_HEIGHT 500
#define MAP_WIDTH 500

#define WALL '#'
#define FLOOR '.'
#define TREE 'T'
#define SOIL '~'
#define ROCK 'o'

typedef struct map_t{
    char map[MAP_HEIGHT][MAP_WIDTH];
    int level;
} map_t;

map_t* map_init(void);
map_t* map_gen(int level, int seed);
void freeMap(map_t* map);
int in_bounds(int x, int y);
void set_tile(map_t* map, int x, int y, char c);
char get_tile(map_t* map, int x, int y);
void fill_map(map_t *map, char c);

#endif
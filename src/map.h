#ifndef MAP_H
#define MAP_H

#include "linked_list.h"
#include "entity.h"

#define MAP_HEIGHT 500
#define MAP_WIDTH 500

typedef struct map_t{
    char map[MAP_HEIGHT][MAP_WIDTH];
    int pos_x, pos_y; // location of player
    struct entity_t* player;
}map_t;


map_t* genMap(void);
void freeMap(map_t* map);
void updateMap(map_t* map);
void add_entity_map(entity_t* entity);



#endif
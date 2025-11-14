#ifndef UNDERWORLD_H
#define UNDERWORLD_H

#include "map.h"

#define MIN_LEAF_SIZE 12
#define MAX_LEAF_SIZE 25
#define MIN_ROOM_SIZE 4
#define MAX_ROOM_SIZE 10

typedef struct bsp_node {
    int x, y, w, h;     // region
    int room_x, room_y, room_w, room_h;
    struct bsp_node* left;
    struct bsp_node* right;
    int has_room;
} bsp_node;


void generate_underworld(map_t* map);

#endif
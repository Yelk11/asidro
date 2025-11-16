#ifndef UNDERWORLD_H
#define UNDERWORLD_H

typedef struct map_t map_t;

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


bsp_node* generate_underworld(map_t* map);
void under_get_player_spawn(map_t* map, int* x, int* y);
void under_get_npc_spawn(map_t* map, int* x, int* y, int px, int py);
#endif
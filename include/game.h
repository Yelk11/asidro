/*
 * This file defines the game_t structure and function prototypes
 * for game management.
 */
#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "actor.h"
#include "sched.h"

typedef struct bsp_node bsp_node;

typedef struct game_t {
	map_t *map;
    sched_node* action_list;
    char ch;
    bsp_node* root;
} game_t;

void initializeGame(game_t* game);
void updateGame(game_t* game);
void freeGame(game_t* game);
void game(void);
bool valid_spawn_tile(map_t* map, int x, int y);
bool place_actor_at(game_t* game, actor_t* npc, int x, int y);
void place_npcs_bsp(game_t* game, bsp_node* root);
void place_npcs_open_map(game_t* game, int count);
#endif // GAME_H

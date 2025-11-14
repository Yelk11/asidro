/*
 * This file defines the game_t structure and function prototypes
 * for game management.
 */
#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "entity.h"
#include "sched.h"

typedef struct game_t {
	map_t *map;
    act_node* action_list;
    char ch;
} game_t;

void initializeGame(game_t* game);
void updateGame(game_t* game);
void freeGame(game_t* game);
void game(void);

#endif // GAME_H

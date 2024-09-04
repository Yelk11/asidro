#ifndef GAME_H
#define GAME_H

#include "map.h"


typedef struct game_t{
    struct map_t* map;
}game_t;

void initializeGame(game_t* game);
void updateGame(game_t* game);
void game(void);



#endif
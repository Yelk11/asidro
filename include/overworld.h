#ifndef OVERWORLD_H
#define OVERWORLD_H

#include "map.h"

void generate_overworld(map_t *map);
void over_get_player_spawn(map_t* map, int* x, int* y);
void over_get_npc_spawn(map_t* map, int* x, int* y, int px, int py);

#endif

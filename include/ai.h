#ifndef AI_H
#define AI_H

#include "actor.h"

int distance_to_player(actor_t* player, actor_t* actor);
void wander_randomly(actor_t* actor);

#endif
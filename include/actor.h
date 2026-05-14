/**
 * @file actor.h
 * @brief Actor helpers built on the ECS component system
 * @author Asidro Team
 * 
 * Provides actor creation and behavior logic for game entities using the
 * ECS world.
 */

#ifndef ACTOR_H
#define ACTOR_H

#include "ecs.h"
#include <stdbool.h>

struct game_t;
typedef struct game_t game_t;

Entity make_actor(actor_type type, int x, int y, int speed);

void player_act(Entity self, game_t* game);
void monster_act(Entity self, game_t* game);
void npc_act(Entity self, game_t* game);

bool actor_attack(Entity attacker, Entity defender);
void actor_take_damage(Entity e, int damage);
bool actor_is_dead(Entity e);

char actor_get_ascii(actor_type type);

#endif

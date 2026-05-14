/**
 * @file sched.h
 * @brief Scheduler using circular doubly-linked list of ECS entities
 * @author Asidro Team
 * 
 * Provides a turn-based scheduling system for managing entity actions.
 */

#ifndef SCHED_H
#define SCHED_H

#include "ecs.h"

struct game_t;
typedef struct game_t game_t;

typedef struct sched_node {
    struct sched_node* next;
    struct sched_node* prev;
    Entity entity;
} sched_node;

sched_node *sched_init(Entity entity);

sched_node* sched_add(sched_node* node, Entity entity);

sched_node* sched_remove(sched_node* node, Entity entity);

Entity sched_current(sched_node *node);

Entity sched_peek(sched_node *node, int num);

void sched_advance(sched_node *node);

sched_node* sched_cycle_actions(sched_node *node, game_t* game);

Entity sched_get_by_id(sched_node* root, Entity id);

Entity sched_get_player(sched_node* root);

Entity sched_get_actor_by_coords(sched_node* root, int x, int y);

sched_node* sched_remove_dead(sched_node* root);

#endif

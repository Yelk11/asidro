#ifndef SCHED_H
#define SCHED_H

#include "actor.h"


typedef struct sched_node{
    struct sched_node* next;
    struct sched_node* prev;
    actor_t* entity;
}sched_node;

sched_node *sched_init(actor_t* actor);

sched_node* sched_add(sched_node* node, actor_t* entity);

sched_node* sched_remove(sched_node* node, actor_t* entity);

actor_t* sched_current(sched_node* node);

actor_t* sched_peek(sched_node* node, int num);

void sched_advance(sched_node* node);

actor_t* sched_get_by_id(sched_node* root, int id);

void sched_cycle_actions(sched_node *node);

actor_t* sched_get_player(sched_node* root);

#endif
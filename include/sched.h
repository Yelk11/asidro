#ifndef SCHED_H
#define SCHED_H

#include "entity.h"


typedef struct act_node{
    struct act_node* next;
    struct act_node* last;
    actor_t* entity;
}act_node;

act_node *sched_init(actor_t* actor);

void sched_add(act_node* node, actor_t* entity, actor_type type);

actor_t* sched_current(act_node* node);

actor_t* sched_peek(act_node* node, int num);

void sched_advance(act_node* node);

#endif
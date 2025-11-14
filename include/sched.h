#ifndef SCHED_H
#define SCHED_H

#include "entity.h"

typedef struct act_node{
    struct act_node* next;
    struct act_node* last;
    entity_t* entity;
}act_node;

act_node *sched_init(entity_t* entity);

entity_t* sched_current(act_node* node);

entity_t* sched_peek(act_node* node, int num);

void sched_advance(act_node* node);

#endif
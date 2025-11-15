#ifndef SCHED_H
#define SCHED_H

#include "actor.h"


typedef struct sched_node{
    struct sched_node* next;
    struct sched_node* last;
    actor_t* entity;
}sched_node;

sched_node *sched_init(actor_t* actor);

void sched_add(sched_node* node, actor_t* entity, actor_type type);

actor_t* sched_current(sched_node* node);

actor_t* sched_peek(sched_node* node, int num);

void sched_advance(sched_node* node);


#endif
#include <stdlib.h>
#include "sched.h"
#include "actor.h"
#include "game.h"

sched_node *sched_init(actor_t* actor)
{
    sched_node *node = calloc(1, sizeof(sched_node));
    node->last = node;
    node->next = node;
    node->entity = actor;
    return node;
}

actor_t *sched_current(sched_node *node)
{
    return node->entity;
}

actor_t *sched_peek(sched_node *node, int num)
{
    sched_node* temp = node;
    for(int i = 0; i < num; i++)
    {
        sched_advance(temp);
    }
    return sched_current(temp);
}

void sched_advance(sched_node *node)
{
    node->entity->act(node->entity);
    node = node->next;
}

#include <stdlib.h>
#include "sched.h"
#include "actor.h"
#include "game.h"

act_node *sched_init(actor_t* actor)
{
    act_node *node = calloc(1, sizeof(act_node));
    node->last = node;
    node->next = node;
    node->entity = actor;
    return node;
}

actor_t *sched_current(act_node *node)
{
    return node->entity;
}

actor_t *sched_peek(act_node *node, int num)
{
    act_node* temp = node;
    for(int i = 0; i < num; i++)
    {
        sched_advance(temp);
    }
    return sched_current(temp);
}

void sched_advance(act_node *node)
{
    node->entity->act(node->entity);
    node = node->next;
}

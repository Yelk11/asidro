#include <stdlib.h>
#include "sched.h"
#include "entity.h"

act_node *sched_init()
{
    act_node *node = calloc(1, sizeof(act_node));
    node->last = node;
    node->next = node;
    node->entity = make_actor('@', 10,10, 100, player_act);
    return node;
}

actor_t *sched_current(act_node *node)
{
    return node->entity;
}

actor_t *sched_peek(act_node *node, int num)
{
}

void sched_advance(act_node *node)
{
    node = node->next;
}

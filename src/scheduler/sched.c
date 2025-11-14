#include <stdlib.h>
#include "sched.h"

act_node *sched_init(entity_t* entity)
{
    act_node *node = calloc(1, sizeof(act_node));
    node->last = node;
    node->next = node;
    node->entity = entity;
    return node;
}

entity_t *sched_current(act_node *node)
{
    return node->entity;
}

entity_t *sched_peek(act_node *node, int num)
{
}

void sched_advance(act_node *node)
{
    node = node->next;
}

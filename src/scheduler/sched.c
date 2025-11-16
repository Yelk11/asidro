#include <stdlib.h>
#include "sched.h"
#include "actor.h"
#include "game.h"

sched_node *sched_init(actor_t* actor)
{
    sched_node *node = calloc(1, sizeof(sched_node));
    node->prev = node;
    node->next = node;
    node->entity = actor;
    return node;
}


sched_node* sched_add(sched_node* node, actor_t* entity)
{
    sched_node* newnode = malloc(sizeof(sched_node));
    newnode->entity = entity;

    if (!node) {
        newnode->next = newnode;
        newnode->prev = newnode;
        return newnode;
    }

    sched_node* nxt = node->next;

    newnode->next = nxt;
    newnode->prev = node;

    node->next = newnode;
    nxt->prev = newnode;

    return newnode;
}


sched_node* sched_remove(sched_node* node, actor_t* entity)
{
    if (!node) return NULL;

    sched_node* cur = node;

    do {
        if (cur->entity == entity) {

            // If this is the only node in the ring
            if (cur->next == cur) {
                free(cur);
                return NULL;  // empty list now
            }

            // Remove cur
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;

            sched_node* new_entry = cur->next;

            free(cur);
            return new_entry;
        }

        cur = cur->next;
    } while (cur != node);

    // Not found
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
        temp = temp->next;
    }
    return sched_current(temp);
}

void sched_advance(sched_node *node)
{
    node = node->next;
}

void sched_cycle_actions(sched_node *node)
{
    sched_node* head = node;
    do{
        node->entity->act(node->entity);
        sched_advance(node);
    }while(head != node);
}

actor_t* sched_get_by_id(sched_node* root, int id)
{
    while(root->entity->id != id)
    {
        sched_advance(root);
    }
    return root->entity;
}

actor_t* sched_get_player(sched_node* root)
{
    if (!root) return NULL;

    sched_node* cur = root;

    do {
        if (cur->entity && cur->entity->type == PLAYER)
            return cur->entity;

        cur = cur->next;
    } while (cur != root);

    return NULL;  // no player in the list
}



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

sched_node* sched_cycle_actions(sched_node *node)
{
    if (!node) return NULL;

    sched_node* head = node;
    sched_node* cur = head;
    do {
        sched_node* next_node = cur->next;

        /* If actor is alive, accumulate energy and act if threshold reached; otherwise remove dead actor */
        if (!actor_is_dead(cur->entity)) {
            actor_t* entity = cur->entity;
            
            /* Accumulate energy based on speed */
            entity->energy += entity->speed;
            
            /* Actor gets a turn if energy >= 100 */
            if (entity->energy >= 100) {
                entity->energy -= 100;
                if (entity->act)
                    entity->act(entity);
            }
        } else {
            head = sched_remove(head, cur->entity);
            if (!head) break;  /* list is now empty */
        }

        cur = next_node;
    } while (cur != head && head != NULL);

    return head;
}

actor_t* sched_get_by_id(sched_node* root, int id)
{
    if (!root) return NULL;
    
    sched_node* cur = root;
    do {
        if (cur->entity && cur->entity->id == id)
            return cur->entity;
        cur = cur->next;
    } while (cur != root);
    
    return NULL;  /* not found */
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

actor_t* sched_get_actor_by_coords(sched_node* root, int x, int y)
{
    if (!root) return NULL;

    sched_node* cur = root;

    do {
        if(cur->entity->x == x && cur->entity->y == y)
            return cur->entity;
        cur = cur->next;
    } while (cur != root);

    return NULL;  // no player in the list
}

sched_node* sched_remove_dead(sched_node* root)
{
    if (!root) return NULL;

    sched_node* cur = root;
    sched_node* head = root;

    do {
        sched_node* next = cur->next;
        if (actor_is_dead(cur->entity)) {
            head = sched_remove(head, cur->entity);
            if (!head) break;
        }
        cur = next;
    } while (cur != head && head != NULL);

    return head;
}
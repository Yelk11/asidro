/**
 * @file sched.c
 * @brief Scheduler implementation for turn-based ECS entities
 * @author Asidro Team
 */

#include <stdlib.h>
#include "sched.h"
#include "actor.h"

sched_node *sched_init(Entity entity)
{
    sched_node *node = calloc(1, sizeof(sched_node));
    if (!node) return NULL;
    node->prev = node;
    node->next = node;
    node->entity = entity;
    return node;
}

sched_node* sched_add(sched_node* node, Entity entity)
{
    sched_node* newnode = malloc(sizeof(sched_node));
    if (!newnode) return NULL;
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

sched_node* sched_remove(sched_node* node, Entity entity)
{
    if (!node) return NULL;
    sched_node* cur = node;
    do {
        if (cur->entity == entity) {
            if (cur->next == cur) {
                free(cur);
                return NULL;
            }
            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;
            sched_node* new_entry = cur->next;
            free(cur);
            return new_entry;
        }
        cur = cur->next;
    } while (cur != node);
    return node;
}

Entity sched_current(sched_node *node)
{
    return node ? node->entity : ECS_INVALID_ENTITY;
}

Entity sched_peek(sched_node *node, int num)
{
    if (!node) return ECS_INVALID_ENTITY;
    sched_node* temp = node;
    for (int i = 0; i < num; i++) {
        temp = temp->next;
    }
    return sched_current(temp);
}

void sched_advance(sched_node *node)
{
    (void)node;
}

static void entity_take_turn(Entity entity, game_t* game)
{
    if (!ecs_is_valid(entity) || !game) return;
    if (ecs_has_component(entity, COMP_PLAYER_INPUT)) {
        player_act(entity, game);
    } else if (ecs_has_component(entity, COMP_AI)) {
        monster_act(entity, game);
    }
}

sched_node* sched_cycle_actions(sched_node *node, game_t* game)
{
    if (!node) return NULL;

    sched_node* head = node;
    sched_node* cur = head;
    do {
        sched_node* next_node = cur->next;
        Entity entity = cur->entity;

        if (!actor_is_dead(entity)) {
            Energy* energy = ecs_get_energy(entity);
            if (energy) {
                energy->energy += energy->speed;
                if (energy->energy >= 100) {
                    energy->energy -= 100;
                    entity_take_turn(entity, game);
                }
            } else {
                entity_take_turn(entity, game);
            }
        } else {
            head = sched_remove(head, entity);
            if (!head) break;
        }

        cur = next_node;
    } while (cur != head && head != NULL);

    return head;
}

Entity sched_get_by_id(sched_node* root, Entity id)
{
    if (!root) return ECS_INVALID_ENTITY;
    sched_node* cur = root;
    do {
        if (cur->entity == id)
            return id;
        cur = cur->next;
    } while (cur != root);
    return ECS_INVALID_ENTITY;
}

Entity sched_get_player(sched_node* root)
{
    if (!root) return ECS_INVALID_ENTITY;
    sched_node* cur = root;
    do {
        if (ecs_get_actor_type(cur->entity) == PLAYER)
            return cur->entity;
        cur = cur->next;
    } while (cur != root);
    return ECS_INVALID_ENTITY;
}

Entity sched_get_actor_by_coords(sched_node* root, int x, int y)
{
    if (!root) return ECS_INVALID_ENTITY;
    sched_node* cur = root;
    do {
        Position* pos = ecs_get_position(cur->entity);
        if (pos && pos->x == x && pos->y == y)
            return cur->entity;
        cur = cur->next;
    } while (cur != root);
    return ECS_INVALID_ENTITY;
}

sched_node* sched_remove_dead(sched_node* root)
{
    if (!root) return NULL;
    sched_node* head = root;
    sched_node* cur = root;

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

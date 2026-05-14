/**
 * @file actor.c
 * @brief Actor behavior implementation on top of ECS
 * @author Asidro Team
 */

#include <stdlib.h>
#include <stdbool.h>

#include "actor.h"
#include "ecs.h"
#include "map.h"
#include "sched.h"
#include "ai.h"

static int sign(int v) { return (v > 0) - (v < 0); }

char actor_get_ascii(actor_type type)
{
    switch(type)
    {
        case PLAYER: return '@';
        case NPC: return 'G';
        case MONSTER: return 'M';
        default: return '?';
    }
}

Entity make_actor(actor_type type, int x, int y, int speed)
{
    Entity entity = ecs_create_entity();
    if (entity == ECS_INVALID_ENTITY) {
        return ECS_INVALID_ENTITY;
    }

    ecs_set_actor_type(entity, type);
    ecs_set_position(entity, x, y);
    ecs_set_renderable(entity, actor_get_ascii(type));
    ecs_set_energy(entity, 50, speed);

    switch (type) {
        case PLAYER:
            ecs_set_health(entity, 20, 20);
            ecs_set_combat(entity, 3);
            ecs_add_component(entity, COMP_PLAYER_INPUT);
            break;
        case NPC:
            ecs_set_health(entity, 5, 5);
            ecs_set_combat(entity, 1);
            ecs_add_component(entity, COMP_AI);
            break;
        case MONSTER:
            ecs_set_health(entity, 8, 8);
            ecs_set_combat(entity, 2);
            ecs_add_component(entity, COMP_AI);
            break;
        default:
            ecs_set_health(entity, 5, 5);
            ecs_set_combat(entity, 1);
            ecs_add_component(entity, COMP_AI);
            break;
    }

    return entity;
}

void player_act(Entity self, game_t* game)
{
    if (!ecs_is_valid(self) || !game) return;

    Position* pos = ecs_get_position(self);
    if (!pos) return;

    int dx = 0;
    int dy = 0;
    switch (game->ch) {
        case 'w': case 'k': dy -= 1; break;
        case 's': case 'j': dy += 1; break;
        case 'a': case 'h': dx -= 1; break;
        case 'd': case 'l': dx += 1; break;
        default: break;
    }

    int new_x = pos->x + dx;
    int new_y = pos->y + dy;
    Entity target = sched_get_actor_by_coords(game->action_list, new_x, new_y);

    if (target != ECS_INVALID_ENTITY && target != self && !actor_is_dead(target)) {
        actor_attack(self, target);
        return;
    }

    if (map_is_walkable(game->map, new_x, new_y)) {
        pos->x = new_x;
        pos->y = new_y;
    }
}

void monster_act(Entity self, game_t* game) {
    if (!ecs_is_valid(self) || !game) return;
    Position* pos = ecs_get_position(self);
    if (!pos) return;

    Entity player = sched_get_player(game->action_list);
    if (player == ECS_INVALID_ENTITY || actor_is_dead(player)) {
        wander_randomly(self, game);
        return;
    }

    int dist = distance_to_player(player, self);
    if (dist <= 1) {
        actor_attack(self, player);
        return;
    }

    if (dist < 10 && dist > 1) {
        Position* player_pos = ecs_get_position(player);
        if (!player_pos) {
            wander_randomly(self, game);
            return;
        }

        int dx = sign(player_pos->x - pos->x);
        int dy = sign(player_pos->y - pos->y);

        int try_x = pos->x + dx;
        int try_y = pos->y;
        if (dx != 0 && map_is_walkable(game->map, try_x, try_y)) {
            Entity occ = sched_get_actor_by_coords(game->action_list, try_x, try_y);
            if (occ == ECS_INVALID_ENTITY || actor_is_dead(occ)) {
                pos->x = try_x;
                pos->y = try_y;
                return;
            }
            if (occ == player) { actor_attack(self, occ); return; }
        }

        try_x = pos->x;
        try_y = pos->y + dy;
        if (dy != 0 && map_is_walkable(game->map, try_x, try_y)) {
            Entity occ = sched_get_actor_by_coords(game->action_list, try_x, try_y);
            if (occ == ECS_INVALID_ENTITY || actor_is_dead(occ)) {
                pos->x = try_x;
                pos->y = try_y;
                return;
            }
            if (occ == player) { actor_attack(self, occ); return; }
        }

        try_x = pos->x + dx;
        try_y = pos->y + dy;
        if (dx != 0 && dy != 0 && map_is_walkable(game->map, try_x, try_y)) {
            Entity occ = sched_get_actor_by_coords(game->action_list, try_x, try_y);
            if (occ == ECS_INVALID_ENTITY || actor_is_dead(occ)) {
                pos->x = try_x;
                pos->y = try_y;
                return;
            }
            if (occ == player) { actor_attack(self, occ); return; }
        }

        wander_randomly(self, game);
        return;
    }

    wander_randomly(self, game);
}

void npc_act(Entity self, game_t* game)
{
    if (!ecs_is_valid(self) || !game) return;
    wander_randomly(self, game);
}

bool actor_attack(Entity attacker, Entity defender)
{
    if (!ecs_is_valid(attacker) || !ecs_is_valid(defender) || actor_is_dead(defender))
        return false;

    Combat* attacker_combat = ecs_get_combat(attacker);
    if (!attacker_combat) return false;

    actor_take_damage(defender, attacker_combat->damage);
    return true;
}

void actor_take_damage(Entity e, int damage)
{
    if (!ecs_is_valid(e) || damage < 0) return;

    Health* health = ecs_get_health(e);
    if (!health) return;

    health->hp -= damage;
    if (health->hp <= 0) {
        health->hp = 0;
        health->alive = false;
    }
}

bool actor_is_dead(Entity e)
{
    Health* health = ecs_get_health(e);
    return !health || !health->alive;
}

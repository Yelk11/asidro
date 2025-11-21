#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>

#include "actor.h"
#include "game.h"
#include "map.h"
#include "ai.h"
#include "screen.h"

static int sign(int v) { return (v > 0) - (v < 0); }

unsigned int next_id(void) {
    static unsigned int counter = 0;
    return counter++;   // returns current value, then increments
}


char actor_get_ascii(actor_type type)
{
    switch(type)
    {
        case PLAYER: return '@'; break;
        case NPC: return 'G'; break;
        case MONSTER: return 'M'; break;
        default: return '?'; break;
    }
}

actor_t* make_actor(actor_type type, int x, int y, int speed, void (*act_fn)(actor_t*), void* data)
{
    actor_t* a = malloc(sizeof(actor_t));
    a->ascii_char = actor_get_ascii(type);
    a->x = x;
    a->y = y;
    a->speed = speed;      // 100 = normal, >100 = faster, <100 = slower
    a->energy = 50;        // start with partial energy (adjusted for balance)
    a->act = act_fn;       // behavior callback
    a->isAlive = true;
    a->type = type;

    /* Initialize health and damage based on type */
    switch (type) {
        case PLAYER:
            a->max_health = 20;
            a->damage = 3;
            break;
        case NPC:
            a->max_health = 5;
            a->damage = 1;
            break;
        case MONSTER:
            a->max_health = 8;
            a->damage = 2;
            break;
        default:
            a->max_health = 5;
            a->damage = 1;
            break;
    }
    a->health = a->max_health;

    a->id = next_id();        // or use a monotonic counter
    a->data = data;
    return a;
}

void player_act(actor_t* self)
{
    game_t* game = (game_t*)self->data;
    int dx = 0;
    int dy = 0;
	switch (game->ch) {
		case 'w': case 'k': dy -= 1; break;
		case 's': case 'j': dy += 1; break;
		case 'a': case 'h': dx -= 1; break;
		case 'd': case 'l': dx += 1; break;
		default: break;
	}

    int new_x = self->x + dx;
    int new_y = self->y + dy;

    /* Check if there's an enemy at the destination */
    actor_t* target = sched_get_actor_by_coords(game->action_list, new_x, new_y);
    if (target && target != self && target->isAlive) {
        actor_attack(self, target);
    } else if (map_is_walkable(game->map, new_x, new_y)) {
        self->x = new_x;
        self->y = new_y;
    }
}


void monster_act(actor_t* self) {
    if (!self) return;
    game_t* game = (game_t*)self->data;
    if (!game) {
        wander_randomly(self);
        return;
    }

    actor_t* player = sched_get_player(game->action_list);
    if (!player) {
        wander_randomly(self);
        return;
    }

    int dist = distance_to_player(player, self);
    if (dist <= 1) {
        /* adjacent: attack the player */
        actor_attack(self, player);
        return;
    }

    if (dist < 10 && dist > 1) {
        /* Step toward player one tile at a time, prefer straight moves */
        int dx = sign(player->x - self->x);
        int dy = sign(player->y - self->y);

        int try_x = self->x + dx;
        int try_y = self->y;
        if (dx != 0 && map_is_walkable(game->map, try_x, try_y)) {
            actor_t* occ = sched_get_actor_by_coords(game->action_list, try_x, try_y);
            if (!occ || !occ->isAlive) {
                self->x = try_x;
                self->y = try_y;
                return;
            }
            /* if occupant is player, attack */
            if (occ == player) { actor_attack(self, occ); return; }
        }

        /* try vertical move */
        try_x = self->x;
        try_y = self->y + dy;
        if (dy != 0 && map_is_walkable(game->map, try_x, try_y)) {
            actor_t* occ = sched_get_actor_by_coords(game->action_list, try_x, try_y);
            if (!occ || !occ->isAlive) {
                self->x = try_x;
                self->y = try_y;
                return;
            }
            if (occ == player) { actor_attack(self, occ); return; }
        }

        /* try diagonal if both dx and dy non-zero */
        try_x = self->x + dx;
        try_y = self->y + dy;
        if ((dx != 0 && dy != 0) && map_is_walkable(game->map, try_x, try_y)) {
            actor_t* occ = sched_get_actor_by_coords(game->action_list, try_x, try_y);
            if (!occ || !occ->isAlive) {
                self->x = try_x;
                self->y = try_y;
                return;
            }
            if (occ == player) { actor_attack(self, occ); return; }
        }

        /* fallback to wandering */
        wander_randomly(self);
        return;
    }

    /* too far: wander */
    wander_randomly(self);
}

void npc_act(actor_t* self)
{

}




bool actor_attack(actor_t* attacker, actor_t* defender)
{
    if (!attacker || !defender || !defender->isAlive)
        return false;

    /* Simple attack: always hits, deals damage */
    actor_take_damage(defender, attacker->damage);
    return true;
}

void actor_take_damage(actor_t* actor, int damage)
{
    if (!actor || damage < 0)
        return;

    actor->health -= damage;
    if (actor->health <= 0) {
        actor->health = 0;
        actor->isAlive = false;
    }
}

bool actor_is_dead(actor_t* actor)
{
    return !actor || !actor->isAlive;
}
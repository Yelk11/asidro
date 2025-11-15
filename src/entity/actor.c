#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>

#include "actor.h"
#include "game.h"

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
    a->energy = 0;         // starts with no accumulated energy
    a->act = act_fn;       // behavior callback
    a->isAlive = true;

    a->id = next_id();        // or use a monotonic counter
    a->data = data;
    return a;
}

void player_act(actor_t* self)
{
    game_t* game = (game_t*)self->data;
    
	switch (game->ch) {
		case 'w': case 'k': self->y -= 1; break;
		case 's': case 'j': self->y += 1; break;
		case 'a': case 'h': self->x -= 1; break;
		case 'd': case 'l': self->x += 1; break;
		default: break;
	}
}

void monster_act(actor_t* self) {
    // simple chasing AI
    // if (distance_to_player(self) < 10) {
    //     move_towards_player(self);
    // } else {
    //     wander_randomly(self);
    // }
}

void npc_act(actor_t* self)
{

}

void place_actor(map_t* map, actor_t* actor)
{
    
}
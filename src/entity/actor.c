#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>

#include "actor.h"
#include "game.h"
#include "map.h"

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
    int dx = 0;
    int dy = 0;
	switch (game->ch) {
		case 'w': case 'k': dy -= 1; break;
		case 's': case 'j': dy += 1; break;
		case 'a': case 'h': dx -= 1; break;
		case 'd': case 'l': dx += 1; break;
		default: break;
	}
    if(map_is_walkable(game->map,self->x + dx, self->y + dy))
    {
        self->x += dx;
        self->y += dy;
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
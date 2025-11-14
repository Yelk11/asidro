#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>

#include "entity.h"
#include "game.h"

/*
actor_t* player = make_actor(10,10, 100, player_act);
actor_t* goblin = make_actor(20,15, 80, monster_act);  // goblin is slower
actor_t* bat    = make_actor(5,5, 150, monster_act);  
*/

actor_t* make_actor(char ascii_char, int x, int y, int speed, void (*act_fn)(actor_t*), void* data)
{
    actor_t* a = malloc(sizeof(actor_t));
    a->ascii_char = ascii_char;
    a->x = x;
    a->y = y;
    a->speed = speed;      // 100 = normal, >100 = faster, <100 = slower
    a->energy = 0;         // starts with no accumulated energy
    a->act = act_fn;       // behavior callback
    a->isAlive = true;

    a->id = rand();        // or use a monotonic counter
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

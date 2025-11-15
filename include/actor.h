#ifndef ACTOR_H
#define ACTOR_H

#include <stdbool.h>

typedef enum {
    PLAYER,
    NPC,
    MONSTER
}actor_type;


typedef struct actor_t{
    int id;
    char ascii_char;
    int x, y;
    int speed;      // higher = more turns
    int energy;     // accumulated
    bool isAlive;
    actor_type type;
    void (*act)(struct actor_t *self);
    void* data;
}actor_t;

typedef enum {
    UP,
    DOWN,
    RIGHT,
    LEFT
}direction;

actor_t* make_actor(actor_type type, int x, int y, int speed, void (*act_fn)(actor_t*), void* data);
void player_act(actor_t* e);
void monster_act(actor_t* self);
void npc_act(actor_t* self);


#endif
#ifndef ENTITY_H
#define ENTITY_H

#include <stdbool.h>

typedef struct actor_t{
    int id;
    char ascii_char;
    int x, y;
    int speed;      // higher = more turns
    int energy;     // accumulated
    bool isAlive;

    void (*act)(struct actor *self);
    void* data;
}actor_t;

typedef enum {
    UP,
    DOWN,
    RIGHT,
    LEFT
}direction;

typedef enum {
    PLAYER,
    NPC,
    MONSTER
}actor_type;







actor_t* make_actor(char ascii_char, int x, int y, int speed, void (*act_fn)(actor_t*), void* data);
void player_act(actor_t* e);
void monster_act(actor_t* self);
void npc_act(actor_t* self);


#endif
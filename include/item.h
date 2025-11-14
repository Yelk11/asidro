#ifndef ITEM_H
#define ITEM_H

#include <stdbool.h>

typedef struct item_t{
    int id;
    char ascii_char;
    int damage;
    int hp;
    int x,y;

    void (*act)(struct item_t *self);
    void* data;
}item_t;


typedef enum {
    WEAPON,
    ARMOUR,
    TOOL,
    MATERIAL
}item_type;


item_t* make_item(char ascii_char, int x, int y, int speed, void (*act_fn)(item_t*), void* data);
void weapon_act(item_t* e);
void armour_act(item_t* self);
void tool_act(item_t* self);
void material_act(item_t* self);

#endif
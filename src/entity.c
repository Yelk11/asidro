#include <stdlib.h>

#include "entity.h"


entity_t* createEntity(position_t start_pos, char charector)
{
  entity_t* newPlayer = calloc(1, sizeof(entity_t));

  newPlayer->pos.y = start_pos.y;
  newPlayer->pos.x = start_pos.x;
  newPlayer->ascii_char = '@';

  return newPlayer;
}


void moveEntity(entity_t* e, char dir)
{
    switch(dir)
    {
        case 'h':
            e->pos.x--; // LEFT
            break;
        case 'j':
            e->pos.y++; // UP
            break;
        case 'k':
            e->pos.y--; // DOWN
            break;
        case 'l':
            e->pos.x++; // RIGHT
            break;
    }
}



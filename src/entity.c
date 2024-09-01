#include <stdlib.h>

#include "entity.h"


entity* createEntity(position start_pos, char charector)
{
  entity* newPlayer = calloc(1, sizeof(entity));

  newPlayer->pos.y = start_pos.y;
  newPlayer->pos.x = start_pos.x;
  newPlayer->ascii_char = '@';

  return newPlayer;
}


void moveEntity(entity* e, char dir)
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



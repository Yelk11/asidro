#include <stdlib.h>

#include "entity.h"

position_t *create_position(int x, int y)
{
    position_t *pos = calloc(1, sizeof(position_t));
    pos->x = x;
    pos->y = y;
    return pos;
}
entity_t *createEntity(position_t *start_pos, char charector)
{
    entity_t *newPlayer = calloc(1, sizeof(entity_t));
    newPlayer->pos = start_pos;
    newPlayer->ascii_char = charector;

    return newPlayer;
}

void moveEntity(entity_t *e, char dir)
{
    switch (dir)
    {
    case 'w':
        e->pos->y--;
        break;
    case 'a':
        e->pos->x--;
        break;
    case 's':
        e->pos->y++;
        break;
    case 'd':
        e->pos->x++;
        break;
    default:
        break;
    }
}

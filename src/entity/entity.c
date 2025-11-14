
#include <stdlib.h>
#include "entity.h"

position_t* create_position(int x, int y)
{
	position_t* p = calloc(1, sizeof(position_t));
	if (!p) return NULL;
	p->x = x;
	p->y = y;
	return p;
}

entity_t* createEntity(position_t* pos, char character)
{
	entity_t* e = calloc(1, sizeof(entity_t));
	if (!e) return NULL;
	e->pos = pos;
	e->ascii_char = character;
	return e;
}

void moveEntity(entity_t* e, char dir)
{
	if (!e || !e->pos) return;
	switch (dir) {
		case 'w': case 'k': e->pos->y -= 1; break;
		case 's': case 'j': e->pos->y += 1; break;
		case 'a': case 'h': e->pos->x -= 1; break;
		case 'd': case 'l': e->pos->x += 1; break;
		default: break;
	}
}




#include "entity.h"
#include "ecs.h"

void move(Entity e)
{
    Position* pos = ecs_get_position(e);
    if (!pos) return;
    pos->x += 1;
}

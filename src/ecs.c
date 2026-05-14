#include "ecs.h"
#include <string.h>

static bool g_entity_active[ECS_MAX_ENTITIES];
static ComponentMask g_entity_mask[ECS_MAX_ENTITIES];
static Position g_positions[ECS_MAX_ENTITIES];
static Renderable g_renderables[ECS_MAX_ENTITIES];
static Combat g_combats[ECS_MAX_ENTITIES];
static Health g_healths[ECS_MAX_ENTITIES];
static Energy g_energies[ECS_MAX_ENTITIES];
static ActorComponent g_actor_types[ECS_MAX_ENTITIES];

void ecs_init(void)
{
    memset(g_entity_active, 0, sizeof(g_entity_active));
    memset(g_entity_mask, 0, sizeof(g_entity_mask));
}

Entity ecs_create_entity(void)
{
    for (Entity e = 0; e < ECS_MAX_ENTITIES; ++e) {
        if (!g_entity_active[e]) {
            g_entity_active[e] = true;
            g_entity_mask[e] = 0;
            g_positions[e] = (Position){0, 0};
            g_renderables[e] = (Renderable){0};
            g_combats[e] = (Combat){0};
            g_healths[e] = (Health){0, 0, false};
            g_energies[e] = (Energy){0, 0};
            g_actor_types[e] = (ActorComponent){0};
            return e;
        }
    }
    return ECS_INVALID_ENTITY;
}

void ecs_destroy_entity(Entity e)
{
    if (!ecs_is_valid(e)) return;
    g_entity_active[e] = false;
    g_entity_mask[e] = 0;
}

bool ecs_is_valid(Entity e)
{
    return e < ECS_MAX_ENTITIES && g_entity_active[e];
}

void ecs_add_component(Entity e, ComponentMask mask)
{
    if (!ecs_is_valid(e)) return;
    g_entity_mask[e] |= mask;
}

void ecs_remove_component(Entity e, ComponentMask mask)
{
    if (!ecs_is_valid(e)) return;
    g_entity_mask[e] &= ~mask;
}

bool ecs_has_component(Entity e, ComponentMask mask)
{
    return ecs_is_valid(e) && (g_entity_mask[e] & mask) == mask;
}

void ecs_set_position(Entity e, int x, int y)
{
    if (!ecs_is_valid(e)) return;
    g_positions[e].x = x;
    g_positions[e].y = y;
    ecs_add_component(e, COMP_POSITION);
}

Position* ecs_get_position(Entity e)
{
    return ecs_has_component(e, COMP_POSITION) ? &g_positions[e] : NULL;
}

void ecs_set_renderable(Entity e, char glyph)
{
    if (!ecs_is_valid(e)) return;
    g_renderables[e].glyph = glyph;
    ecs_add_component(e, COMP_RENDERABLE);
}

Renderable* ecs_get_renderable(Entity e)
{
    return ecs_has_component(e, COMP_RENDERABLE) ? &g_renderables[e] : NULL;
}

void ecs_set_combat(Entity e, int damage)
{
    if (!ecs_is_valid(e)) return;
    g_combats[e].damage = damage;
    ecs_add_component(e, COMP_COMBAT);
}

Combat* ecs_get_combat(Entity e)
{
    return ecs_has_component(e, COMP_COMBAT) ? &g_combats[e] : NULL;
}

void ecs_set_health(Entity e, int hp, int max_hp)
{
    if (!ecs_is_valid(e)) return;
    g_healths[e].hp = hp;
    g_healths[e].max_hp = max_hp;
    g_healths[e].alive = (hp > 0);
    ecs_add_component(e, COMP_HEALTH);
}

Health* ecs_get_health(Entity e)
{
    return ecs_has_component(e, COMP_HEALTH) ? &g_healths[e] : NULL;
}

void ecs_set_actor_type(Entity e, actor_type type)
{
    if (!ecs_is_valid(e)) return;
    g_actor_types[e].type = type;
    ecs_add_component(e, COMP_ACTOR_TYPE);
}

actor_type ecs_get_actor_type(Entity e)
{
    if (!ecs_is_valid(e) || !ecs_has_component(e, COMP_ACTOR_TYPE))
        return MONSTER;
    return g_actor_types[e].type;
}

void ecs_set_energy(Entity e, int energy, int speed)
{
    if (!ecs_is_valid(e)) return;
    g_energies[e].energy = energy;
    g_energies[e].speed = speed;
    ecs_add_component(e, COMP_ENERGY);
}

Energy* ecs_get_energy(Entity e)
{
    return ecs_has_component(e, COMP_ENERGY) ? &g_energies[e] : NULL;
}

Entity ecs_find_player(void)
{
    for (Entity e = 0; e < ECS_MAX_ENTITIES; ++e) {
        if (ecs_is_valid(e) && ecs_has_component(e, COMP_ACTOR_TYPE) &&
            g_actor_types[e].type == PLAYER) {
            return e;
        }
    }
    return ECS_INVALID_ENTITY;
}

Entity ecs_entity_at(int x, int y)
{
    for (Entity e = 0; e < ECS_MAX_ENTITIES; ++e) {
        if (ecs_is_valid(e) && ecs_has_component(e, COMP_POSITION)) {
            if (g_positions[e].x == x && g_positions[e].y == y) {
                return e;
            }
        }
    }
    return ECS_INVALID_ENTITY;
}

void ecs_each(ComponentMask required, void (*fn)(Entity e, void* ctx), void* ctx)
{
    if (!fn) return;
    for (Entity e = 0; e < ECS_MAX_ENTITIES; ++e) {
        if (ecs_is_valid(e) && (g_entity_mask[e] & required) == required) {
            fn(e, ctx);
        }
    }
}

#ifndef ECS_H
#define ECS_H

#include <stdint.h>
#include <stdbool.h>

#define ECS_MAX_ENTITIES 256u
#define ECS_INVALID_ENTITY UINT32_MAX

typedef uint32_t Entity;

/**
 * @enum actor_type
 * @brief Classification of entity actors in the game
 */
typedef enum {
    PLAYER,
    NPC,
    MONSTER
} actor_type;

/**
 * @brief ECS component bitmask flags
 */
typedef uint32_t ComponentMask;

enum {
    COMP_POSITION     = 1u << 0,
    COMP_RENDERABLE   = 1u << 1,
    COMP_COMBAT       = 1u << 2,
    COMP_HEALTH       = 1u << 3,
    COMP_AI           = 1u << 4,
    COMP_PLAYER_INPUT = 1u << 5,
    COMP_ENERGY       = 1u << 6,
    COMP_ACTOR_TYPE   = 1u << 7
};

/**
 * @brief Position component for entity placement
 */
typedef struct {
    int x;
    int y;
} Position;

/**
 * @brief Renderable component for display glyphs
 */
typedef struct {
    char glyph;
} Renderable;

/**
 * @brief Combat component for damage values
 */
typedef struct {
    int damage;
} Combat;

/**
 * @brief Health component for hit points and alive state
 */
typedef struct {
    int hp;
    int max_hp;
    bool alive;
} Health;

/**
 * @brief Energy component for turn scheduling
 */
typedef struct {
    int energy;
    int speed;
} Energy;

/**
 * @brief Actor component for type classification
 */
typedef struct {
    actor_type type;
} ActorComponent;

void ecs_init(void);
Entity ecs_create_entity(void);
void ecs_destroy_entity(Entity e);
bool ecs_is_valid(Entity e);

void ecs_add_component(Entity e, ComponentMask mask);
void ecs_remove_component(Entity e, ComponentMask mask);
bool ecs_has_component(Entity e, ComponentMask mask);

void ecs_set_position(Entity e, int x, int y);
Position* ecs_get_position(Entity e);

void ecs_set_renderable(Entity e, char glyph);
Renderable* ecs_get_renderable(Entity e);

void ecs_set_combat(Entity e, int damage);
Combat* ecs_get_combat(Entity e);

void ecs_set_health(Entity e, int hp, int max_hp);
Health* ecs_get_health(Entity e);

void ecs_set_actor_type(Entity e, actor_type type);
actor_type ecs_get_actor_type(Entity e);

void ecs_set_energy(Entity e, int energy, int speed);
Energy* ecs_get_energy(Entity e);

Entity ecs_find_player(void);
Entity ecs_entity_at(int x, int y);
void ecs_each(ComponentMask required, void (*fn)(Entity e, void* ctx), void* ctx);

#endif // ECS_H

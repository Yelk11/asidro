/**
 * @file actor.h
 * @brief Actor system for managing game entities (player, NPCs, monsters)
 * @author Asidro Team
 * 
 * Defines actor structures, types, and functions for managing game entities
 * including player character, NPCs, and monsters. Provides combat mechanics
 * and actor lifecycle management.
 */

#ifndef ACTOR_H
#define ACTOR_H

#include <stdbool.h>

/**
 * @enum actor_type
 * @brief Classification of actors in the game
 */
typedef enum {
    PLAYER,     /**< Player character controlled by user */
    NPC,        /**< Non-player character */
    MONSTER     /**< Enemy entity */
}actor_type;


/**
 * @struct actor_t
 * @brief Represents a game entity (player, NPC, or monster)
 * 
 * Contains all state information for an actor including position, health,
 * combat statistics, and behavior callback function.
 */
typedef struct actor_t{
    int id;                             /**< Unique actor identifier */
    char ascii_char;                    /**< Character displayed on screen */
    int x, y;                           /**< Position on map */
    int speed;                          /**< Energy gain per turn (100 = normal) */
    int energy;                         /**< Accumulated energy for turn system */
    bool isAlive;                       /**< Living status */
    actor_type type;                    /**< Actor classification */
    int health;                         /**< Current hit points */
    int max_health;                     /**< Maximum hit points */
    int damage;                         /**< Attack damage value */
    void (*act)(struct actor_t *self);  /**< Behavior callback function */
    void* data;                         /**< Pointer to game state (game_t*) */
}actor_t;

/**
 * @enum direction
 * @brief Cardinal directions for movement
 */
typedef enum {
    UP,     /**< Move up (negative Y) */
    DOWN,   /**< Move down (positive Y) */
    RIGHT,  /**< Move right (positive X) */
    LEFT    /**< Move left (negative X) */
}direction;

/**
 * @brief Create a new actor
 * 
 * Allocates and initializes an actor with specified parameters. Sets default
 * health, damage, and other properties based on actor type.
 * 
 * @param type Actor classification (PLAYER, NPC, or MONSTER)
 * @param x Starting X coordinate
 * @param y Starting Y coordinate
 * @param speed Energy gain per turn (100 = standard speed)
 * @param act_fn Behavior callback function
 * @param data Pointer to game state (typically game_t*)
 * 
 * @return Pointer to newly allocated actor_t, or NULL on allocation failure
 * 
 * @note Caller is responsible for freeing the returned pointer
 * 
 * @see actor_t, actor_type
 */
actor_t* make_actor(actor_type type, int x, int y, int speed, void (*act_fn)(actor_t*), void* data);

/**
 * @brief Player action callback
 * 
 * Handles player input and movement. Reads input from game state,
 * processes movement and combat actions.
 * 
 * @param e Player actor to process
 * 
 * @note Called once per player turn
 * @see game_t.ch for current input character
 */
void player_act(actor_t* e);

/**
 * @brief Monster action callback
 * 
 * Implements monster AI behavior including:
 * - Attacking player if adjacent
 * - Chasing player if nearby
 * - Wandering randomly otherwise
 * 
 * @param self Monster actor to process
 * 
 * @note Called once per monster turn
 * @see distance_to_player, actor_attack
 */
void monster_act(actor_t* self);

/**
 * @brief NPC action callback
 * 
 * Placeholder for NPC-specific behavior. Currently unused.
 * 
 * @param self NPC actor to process
 */
void npc_act(actor_t* self);

/**
 * @defgroup Combat Combat Functions
 * @brief Functions for managing combat between actors
 * @{
 */

/**
 * @brief One actor attacks another
 * 
 * Performs an attack action where attacker deals damage to defender.
 * Automatically applies damage and checks for death.
 * 
 * @param attacker Actor initiating the attack
 * @param defender Actor receiving the attack
 * 
 * @return true if attack was successful, false if attack failed or defender is invalid
 * 
 * @note Automatically calls actor_take_damage() on defender
 * @see actor_take_damage, actor_is_dead
 */
bool actor_attack(actor_t* attacker, actor_t* defender);

/**
 * @brief Apply damage to an actor
 * 
 * Reduces actor's health by specified amount. Sets isAlive to false
 * if health drops to 0 or below.
 * 
 * @param actor Actor receiving damage
 * @param damage Amount of damage to apply
 * 
 * @note Health cannot go below 0
 * @see actor_is_dead
 */
void actor_take_damage(actor_t* actor, int damage);

/**
 * @brief Check if an actor is dead
 * 
 * Determines if an actor is no longer alive. Returns true for
 * NULL actors or actors with isAlive = false.
 * 
 * @param actor Actor to check
 * 
 * @return true if actor is dead or NULL, false otherwise
 */
bool actor_is_dead(actor_t* actor);

/** @} */

#endif
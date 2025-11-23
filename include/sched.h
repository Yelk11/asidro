/**
 * @file sched.h
 * @brief Actor scheduler using circular doubly-linked list
 * @author Asidro Team
 * 
 * Provides a turn-based scheduling system for managing actor actions.
 * Implements an energy-based system where actors accumulate energy each
 * cycle and take turns when energy reaches threshold.
 */

#ifndef SCHED_H
#define SCHED_H

#include "actor.h"

/**
 * @struct sched_node
 * @brief Node in the scheduler ring
 * 
 * Part of a circular doubly-linked list structure that represents
 * the queue of actors in the game.
 */
typedef struct sched_node{
    struct sched_node* next;    /**< Next node in ring */
    struct sched_node* prev;    /**< Previous node in ring */
    actor_t* entity;            /**< Actor stored in this node */
}sched_node;

/**
 * @brief Initialize scheduler with first actor
 * 
 * Creates a new scheduler ring with a single actor as the head.
 * The ring initially contains only this actor pointing to itself.
 * 
 * @param actor First actor (typically the player)
 * 
 * @return Pointer to new scheduler head, or NULL on allocation failure
 * 
 * @note Caller must eventually free the returned pointer and all added nodes
 * @see sched_add, sched_remove
 */
sched_node *sched_init(actor_t* actor);

/**
 * @brief Add an actor to the scheduler
 * 
 * Inserts a new actor into the scheduler ring after the given node.
 * If node is NULL or empty, creates a new ring.
 * 
 * @param node Current position in ring (insertion point)
 * @param entity Actor to add
 * 
 * @return Updated head (may differ if ring was empty), or NULL on failure
 * 
 * @note Returns new head if ring was empty; otherwise may return original head
 * @see sched_init, sched_remove
 */
sched_node* sched_add(sched_node* node, actor_t* entity);

/**
 * @brief Remove an actor from the scheduler
 * 
 * Searches the entire ring for the specified actor and removes it.
 * Properly maintains ring structure even if removing only/last node.
 * 
 * @param node Any node in the ring (search starts here)
 * @param entity Actor to remove
 * 
 * @return Updated head (or NULL if ring is now empty)
 * 
 * @note Frees the removed node's memory
 * @note If removing the head, returns next actor as new head
 * @see sched_add
 */
sched_node* sched_remove(sched_node* node, actor_t* entity);

/**
 * @brief Get the actor at current position
 * 
 * Simple accessor for getting the actor stored in a scheduler node.
 * 
 * @param node Scheduler node to query
 * 
 * @return Actor pointer stored in node
 * 
 * @see sched_peek
 */
actor_t* sched_current(sched_node* node);

/**
 * @brief Peek ahead in the scheduler
 * 
 * Returns the actor at position n nodes ahead in the ring.
 * 
 * @param node Starting position
 * @param num Number of nodes to advance (0 = current)
 * 
 * @return Actor at that position
 * 
 * @see sched_current
 */
actor_t* sched_peek(sched_node* node, int num);

/**
 * @brief Advance scheduler position to next node
 * 
 * Moves to the next actor in the ring (typically not used directly).
 * 
 * @param node Node to advance from
 * 
 * @note This is a stub - direct node->next access is often used instead
 */
void sched_advance(sched_node* node);

/**
 * @brief Find actor by unique ID
 * 
 * Searches the scheduler ring for an actor with the specified ID.
 * 
 * @param root Scheduler head to start search
 * @param id Actor ID to find
 * 
 * @return Actor pointer if found, NULL otherwise
 * 
 * @see sched_get_player, sched_get_actor_by_coords
 */
actor_t* sched_get_by_id(sched_node* root, int id);

/**
 * @brief Process one cycle of actor turns
 * 
 * Iterates through all actors, accumulating energy and executing turns
 * when energy threshold (100) is reached. Also removes dead actors.
 * 
 * Energy System:
 * - Each actor gains energy equal to their speed per cycle
 * - When energy >= 100, actor takes a turn and energy -= 100
 * - This creates variable turn rates based on speed stat
 * 
 * @param node Scheduler head
 * 
 * @return Updated head (which may change if removals occur)
 * 
 * @note Calls each actor's act() callback if they have a turn
 * @note Dead actors are automatically removed
 * @see actor_t.act, actor_is_dead
 */
sched_node* sched_cycle_actions(sched_node *node);

/**
 * @brief Find the player actor
 * 
 * Searches the scheduler ring for the player actor (type == PLAYER).
 * 
 * @param root Scheduler head to start search
 * 
 * @return Player actor pointer if found, NULL otherwise
 * 
 * @note There should always be exactly one player in the scheduler
 * @see sched_get_by_id, sched_get_actor_by_coords
 */
actor_t* sched_get_player(sched_node* root);

/**
 * @brief Find actor at specific map coordinates
 * 
 * Searches the scheduler ring for an actor at the given position.
 * 
 * @param root Scheduler head to start search
 * @param x X coordinate
 * @param y Y coordinate
 * 
 * @return Actor at position if found, NULL otherwise
 * 
 * @note Returns first actor found at location if multiple (shouldn't happen)
 * @see sched_get_player, sched_get_by_id
 */
actor_t* sched_get_actor_by_coords(sched_node* root, int x, int y);

/**
 * @brief Remove all dead actors from scheduler
 * 
 * Scans the scheduler ring and removes all dead actors.
 * Efficient single-pass cleanup operation.
 * 
 * @param root Scheduler head
 * 
 * @return Updated head (or NULL if all actors are dead)
 * 
 * @note Used to clean up corpses after combat
 * @see actor_is_dead, sched_remove
 */
sched_node* sched_remove_dead(sched_node* root);

#endif
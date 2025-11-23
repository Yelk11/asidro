/**
 * @file screen.h
 * @brief Display and rendering system using ncurses
 * @author Asidro Team
 * 
 * Provides functions for rendering the game UI including the game viewport,
 * status windows, and game-over screen.
 */

#ifndef SCREEN_H
#define SCREEN_H

#include "game.h"

/**
 * @brief Update all UI elements and render to screen
 * 
 * Refreshes the three main windows: game viewport, player status bar,
 * and game statistics window. Should be called once per game loop.
 * 
 * Layout:
 * ```
 * ┌─────────────────┬──────────┐
 * │                 │ STATS    │
 * │   GAME VIEW     │ WINDOW   │
 * │  (map + actors) │          │
 * ├─────────────────┴──────────┤
 * │   PLAYER STATUS WINDOW     │
 * │  HP: [##########----] 15/20│
 * └────────────────────────────┘
 * ```
 * 
 * @param game Game state to render
 * 
 * @note Uses werase() + wrefresh() for efficient incremental updates
 * @note Windows are created once on first call and reused
 * @see draw_game, draw_player_stat, draw_game_stat
 */
void update_screen(game_t* game);

/**
 * @brief Display game-over screen and message
 * 
 * Shows "GAME OVER!" centered on screen with instructions to press Q.
 * Clears all windows and uses the full terminal display.
 * 
 * @note No parameters - uses ncurses stdscr directly
 * @note Blocks until handled by game loop (which waits for Q key)
 * 
 * @see updateGame
 */
void draw_game_over_screen();

#endif

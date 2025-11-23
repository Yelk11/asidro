/**
 * @file main.c
 * @brief Program entry point
 * @author Asidro Team
 * 
 * Initializes ncurses and starts the game. Handles terminal setup,
 * cleanup, and main game execution flow.
 */

#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "game.h"
#include "map.h"

/**
 * @brief Main program entry point
 * 
 * Initializes ncurses terminal mode, starts the game loop, and
 * performs cleanup on exit.
 * 
 * @param argc Argument count (unused)
 * @param argv Argument vector (unused)
 * 
 * @return 0 on normal exit
 * 
 * @note Requires ncurses library to be linked
 */
int main(int argc, char **argv)
{
    initscr();
    raw();
    noecho();

    game();

    endwin();
    return 0;
}





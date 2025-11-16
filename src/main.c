#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "game.h"
#include "map.h"
int main(int argc, char **argv)
{
    initscr();
    raw();
    noecho();

    game();

    endwin();
    return 0;
}





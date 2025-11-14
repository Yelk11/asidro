#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "game.h"
#include "map.h"
int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "--debug") == 0) {
        /* Debug mode: generate map and print a viewport to stdout (no ncurses).
         * If the terminal size can be obtained we print that many columns/rows
         * (clamped to map dimensions) centered on the map; otherwise we fall
         * back to 80x25. This avoids dumping huge maps to the terminal.
         */
        int term_w = 80, term_h = 25;
        struct winsize ws;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) {
            if (ws.ws_col > 0) term_w = ws.ws_col;
            if (ws.ws_row > 0) term_h = ws.ws_row;
        }

        map_t *map = map_create();

        int view_w = term_w;
        int view_h = term_h;
        if (view_w > MAP_WIDTH) view_w = MAP_WIDTH;
        if (view_h > MAP_HEIGHT) view_h = MAP_HEIGHT;

        int start_x = 0;
        int start_y = 0;
        /* center viewport on map */
        if (MAP_WIDTH > view_w) start_x = (MAP_WIDTH - view_w) / 2;
        if (MAP_HEIGHT > view_h) start_y = (MAP_HEIGHT - view_h) / 2;

        for (int y = start_x ? start_y : start_y; y < start_y + view_h; y++) {
            for (int x = start_x; x < start_x + view_w; x++) {
                putchar(map->map[y][x]);
            }
            putchar('\n');
        }

        freeMap(map);
        return 0;
    }

    initscr();
    raw();
    noecho();

    game();

    endwin();
    return 0;
}





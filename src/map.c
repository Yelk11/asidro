#include <ncurses.h>

#include "map.h"


#define GRASS     ' '
#define EMPTY     '.'
#define WATER     '~'
#define MOUNTAIN  '^'
#define PLAYER    '*'

map_t* genMap(void)
{
    int y, x;
    map_t* map = calloc(1, sizeof(map_t));

    /* draw the quest map */

    /* background */

    for (y = 0; y < LINES; y++) {
        mvhline(y, 0, GRASS, COLS);
    }

    /* mountains, and mountain path */

    for (x = COLS / 2; x < COLS * 3 / 4; x++) {
        mvvline(0, x, MOUNTAIN, LINES);
    }

    mvhline(LINES / 4, 0, GRASS, COLS);

    /* lake */

    for (y = 1; y < LINES / 2; y++) {
        mvhline(y, 1, WATER, COLS / 3);
    }
    return map;
}

void updateMap(map_t* map)
{
    
}

void freeMap(map_t* map)
{

}

void add_entity_map(entity_t* entity)
{
    
}
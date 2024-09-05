#include "screen.h"
#include "ncurses.h"



void update_screen(map_t* map)
{
    int my, mx;
    getmaxyx(stdscr, my, mx);
    for(int x = 0; x < mx; x++)
    {
        for(int y = 0; y < my; y++)
        {
            if(map->map[y][x])
            {
                mvaddch(y,x,map->map[y][x]);
            }else{
                mvaddch(y,x,'X');
            }
        }
    }
    refresh();
}




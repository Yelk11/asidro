#include "screen.h"
#include "ncurses.h"



void update_screen(map_t* map)
{
    int my, mx;
    getmaxyx(stdscr, mx, my);
    for(int x = 0; x < mx; x++)
    {
        for(int y = 0; y < my; y++)
        {
            if(map->map[y][x])
            {
                mvaddch(x,y,map->map[y][x]);
            }else{
                mvaddch(x,y,'X');
            }
            
        }
    }
    refresh();
}




#include "screen.h"
#include "ncurses.h"



void update_screen(game_t* game)
{
    int my, mx;
    getmaxyx(stdscr, my, mx);
    for(int x = 0; x < mx; x++)
    {
        for(int y = 0; y < my; y++)
        {
            if(game->map->map[y][x])
            {
                mvaddch(y,x,game->map->map[y][x]);
            }else{
                mvaddch(y,x,'X');
            }
        }
    }
    
    refresh();
}




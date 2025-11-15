#include "screen.h"
#include "ncurses.h"
#include "map.h"



void update_screen(game_t* game)
{
    int scr_h, scr_w;
    getmaxyx(stdscr, scr_h, scr_w);

    int px = game->player->x;
    int py = game->player->y;

    // ---- CAMERA CALCULATION ----
    int cam_x = px - scr_w / 2;
    int cam_y = py - scr_h / 2;

    // Clamp camera to map bounds safely
    if (cam_x < 0) cam_x = 0;
    if (cam_y < 0) cam_y = 0;
    cam_x = (cam_x > MAP_WIDTH - scr_w) ? ((MAP_WIDTH - scr_w > 0) ? MAP_WIDTH - scr_w : 0) : cam_x;
    cam_y = (cam_y > MAP_HEIGHT - scr_h) ? ((MAP_HEIGHT - scr_h > 0) ? MAP_HEIGHT - scr_h : 0) : cam_y;

    // ---- DRAW THE MAP WINDOW ----
    for (int y = 0; y < scr_h; y++)
    {
        for (int x = 0; x < scr_w; x++)
        {
            int map_x = cam_x + x;
            int map_y = cam_y + y;

            char ch = 'X'; // default
            if (map_y >= 0 && map_y < MAP_HEIGHT &&
                map_x >= 0 && map_x < MAP_WIDTH)
            {
                ch = game->map->map[map_y][map_x];
                if (!ch) ch = 'X';
            }

            mvaddch(y, x, ch);
        }
    }
    int draw_x = px - cam_x;
    int draw_y = py - cam_y;
    /* Draw actors */
    sched_node* current = game->action_list;
    do {
        if (draw_x >= 0 && draw_x < scr_w && draw_y >= 0 && draw_y < scr_h)
            mvaddch(draw_y, draw_x, current->entity->ascii_char);
        sched_advance(current);
    } while (current != game->action_list);
    // ---- DRAW PLAYER WITH CAMERA OFFSET ----
    
    if (draw_x >= 0 && draw_x < scr_w && draw_y >= 0 && draw_y < scr_h)
        mvaddch(draw_y, draw_x, game->player->ascii_char);

    refresh();
}

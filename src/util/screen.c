#include "screen.h"
#include "ncurses.h"
#include "map.h"

void draw_game(WINDOW* win, game_t* game)
{
    int scr_h, scr_w;
    getmaxyx(win, scr_h, scr_w);
    actor_t* p = sched_get_player(game->action_list);
    int cam_x = p->x - scr_w / 2;
    int cam_y = p->y - scr_h / 2;

    // Clamp camera to map bounds safely
    if (cam_x < 0) cam_x = 0;
    if (cam_y < 0) cam_y = 0;
    if (cam_x > MAP_WIDTH - scr_w) 
        cam_x = (MAP_WIDTH - scr_w > 0) ? MAP_WIDTH - scr_w : 0;
    if (cam_y > MAP_HEIGHT - scr_h) 
        cam_y = (MAP_HEIGHT - scr_h > 0) ? MAP_HEIGHT - scr_h : 0;


    for (int y = 0; y < scr_h; y++) {
        for (int x = 0; x < scr_w; x++) {
            int map_x = cam_x + x;
            int map_y = cam_y + y;

            char ch = 'X'; /* default */
            if (map_y >= 0 && map_y < MAP_HEIGHT &&
                map_x >= 0 && map_x < MAP_WIDTH) {
                ch = game->map->map[map_y][map_x];
                if (!ch) ch = 'X';
            }

            mvwaddch(win,y, x, ch);
        }
    }
    /* Draw actors in main window */
    sched_node *cur = game->action_list;
    sched_node *head = cur;
    int draw_x = p->x - cam_x;
    int draw_y = p->y - cam_y;

    if (cur) {
        do {
            actor_t *a = cur->entity;
            int ax = a->x - cam_x;
            int ay = a->y - cam_y;

            if (ax >= 0 && ax < scr_w && ay >= 0 && ay < scr_h)
                mvwaddch(win,ay, ax, a->ascii_char);

            cur = cur->next;
        } while (cur != head);
    }

    /* Draw player */
    if (draw_x >= 0 && draw_x < scr_w && draw_y >= 0 && draw_y < scr_h)
        mvwaddch(win, draw_y, draw_x, p->ascii_char);

}

void draw_player_stat(WINDOW* win, game_t* game)
{
    int scr_h, scr_w;
    getmaxyx(win, scr_h, scr_w);
    actor_t* p = sched_get_player(game->action_list);
    
    /* Clear window and draw status */
    wclear(win);
    mvwaddstr(win, 0, 0, "=== PLAYER STATUS ===");
    
    /* Health bar visualization */
    int bar_width = 15;
    int filled = (p->health * bar_width) / p->max_health;
    if (filled > bar_width) filled = bar_width;
    
    mvwaddstr(win, 1, 0, "HP: [");
    for (int i = 0; i < bar_width; i++) {
        waddch(win, i < filled ? '#' : '-');
    }
    mvwprintw(win, 1, 5 + bar_width + 1, "] %d/%d", p->health, p->max_health);
}

void draw_game_stat(WINDOW* win, game_t* game)
{
    int scr_h, scr_w;
    getmaxyx(win, scr_h, scr_w);
    actor_t* p = sched_get_player(game->action_list);

    wclear(win);
    mvwaddstr(win, 0, 1, "--- STATS ---");
    mvwprintw(win, 1, 1, "HP: %d/%d", p->health, p->max_health);
    mvwprintw(win, 2, 1, "DMG: %d", p->damage);
    mvwprintw(win, 3, 1, "LV: 1");
    mvwaddstr(win, 5, 1, "--- MESSAGES ---");
}


void update_screen(game_t *game)
{
    WINDOW *game_win;
    WINDOW *player_stat_win;
    WINDOW *game_stat_win;
    int scr_h, scr_w;
    getmaxyx(stdscr, scr_h, scr_w);


    /* Window layout:
     * - Top-left: game viewport (largest)
     * - Right: stats/message window
     * - Bottom: health/stat bar
     */
    int stat_win_w = 25;  /* right side width */
    int health_bar_h = 3; /* bottom height */
    int game_win_w = scr_w - stat_win_w;
    int game_win_h = scr_h - health_bar_h;

    player_stat_win = newwin(health_bar_h, scr_w, game_win_h, 0);
    game_stat_win = newwin(scr_h - health_bar_h, stat_win_w, 0, game_win_w);
    game_win = newwin(game_win_h, game_win_w, 0, 0);


    draw_game(game_win, game);
    draw_player_stat(player_stat_win, game);
    draw_game_stat(game_stat_win, game);
    wrefresh(game_stat_win);
    wrefresh(player_stat_win);
    wrefresh(game_win);

}



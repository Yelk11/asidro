#include "screen.h"
#include "ncurses.h"
#include "map.h"
#include "ecs.h"
#include "sched.h"

static WINDOW *game_win = NULL;
static WINDOW *player_stat_win = NULL;
static WINDOW *game_stat_win = NULL;

void init_screen_windows(int scr_h, int scr_w)
{
    if (game_win) delwin(game_win);
    if (player_stat_win) delwin(player_stat_win);
    if (game_stat_win) delwin(game_stat_win);

    int stat_win_w = 25;
    int health_bar_h = 3;
    int game_win_w = scr_w - stat_win_w;
    int game_win_h = scr_h - health_bar_h;

    player_stat_win = newwin(health_bar_h, scr_w, game_win_h, 0);
    game_stat_win = newwin(scr_h - health_bar_h, stat_win_w, 0, game_win_w);
    game_win = newwin(game_win_h, game_win_w, 0, 0);

    box(game_win, 0, 0);
    box(player_stat_win, 0, 0);
    box(game_stat_win, 0, 0);
}

void draw_game(WINDOW* win, game_t* game)
{
    int scr_h, scr_w;
    getmaxyx(win, scr_h, scr_w);

    Entity player = sched_get_player(game->action_list);
    if (player == ECS_INVALID_ENTITY || actor_is_dead(player)) {
        draw_game_over_screen();
        return;
    }

    Position* player_pos = ecs_get_position(player);
    if (!player_pos) return;

    int cam_x = player_pos->x - scr_w / 2;
    int cam_y = player_pos->y - scr_h / 2;

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
            char ch = 'X';
            if (map_y >= 0 && map_y < MAP_HEIGHT && map_x >= 0 && map_x < MAP_WIDTH) {
                ch = game->map->map[map_y][map_x];
                if (!ch) ch = 'X';
            }
            mvwaddch(win, y, x, ch);
        }
    }

    if (game->action_list) {
        sched_node* cur = game->action_list;
        do {
            Entity e = cur->entity;
            Position* pos = ecs_get_position(e);
            Renderable* render = ecs_get_renderable(e);
            if (pos && render) {
                int ax = pos->x - cam_x;
                int ay = pos->y - cam_y;
                if (ax >= 0 && ax < scr_w && ay >= 0 && ay < scr_h) {
                    mvwaddch(win, ay, ax, render->glyph);
                }
            }
            cur = cur->next;
        } while (cur != game->action_list);
    }
}

void draw_player_stat(WINDOW* win, game_t* game)
{
    Entity player = sched_get_player(game->action_list);
    Health* health = ecs_get_health(player);

    if (player == ECS_INVALID_ENTITY || !health) {
        return;
    }

    werase(win);
    mvwaddstr(win, 0, 0, "=== PLAYER STATUS ===");

    int bar_width = 15;
    int filled = (health->hp * bar_width) / health->max_hp;
    if (filled > bar_width) filled = bar_width;

    mvwaddstr(win, 1, 0, "HP: [");
    for (int i = 0; i < bar_width; i++) {
        waddch(win, i < filled ? '#' : '-');
    }
    mvwprintw(win, 1, 5 + bar_width + 1, "] %d/%d", health->hp, health->max_hp);
}

void draw_game_stat(WINDOW* win, game_t* game)
{
    Entity player = sched_get_player(game->action_list);
    Health* health = ecs_get_health(player);
    Combat* combat = ecs_get_combat(player);

    if (player == ECS_INVALID_ENTITY || !health || !combat) {
        return;
    }

    werase(win);
    mvwaddstr(win, 0, 1, "--- STATS ---");
    mvwprintw(win, 1, 1, "HP: %d/%d", health->hp, health->max_hp);
    mvwprintw(win, 2, 1, "DMG: %d", combat->damage);
    mvwprintw(win, 3, 1, "LV: 1");
    mvwaddstr(win, 5, 1, "--- MESSAGES ---");
}

void update_screen(game_t *game)
{
    int scr_h, scr_w;
    getmaxyx(stdscr, scr_h, scr_w);

    static int initialized = 0;
    if (!initialized) {
        init_screen_windows(scr_h, scr_w);
        initialized = 1;
    }

    draw_game(game_win, game);
    draw_player_stat(player_stat_win, game);
    draw_game_stat(game_stat_win, game);

    wrefresh(game_win);
    wrefresh(player_stat_win);
    wrefresh(game_stat_win);
}

void draw_game_over_screen()
{
    clear();
    mvprintw(LINES / 2 - 1, (COLS - 10) / 2, "GAME OVER!");
    mvprintw(LINES / 2 + 1, (COLS - 18) / 2, "Press Q to exit...");
    refresh();
}

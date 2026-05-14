#include <stdlib.h>
#include <stdio.h>

#include "game.h"
#include "game_cfg.h"
#include "map.h"
#include "actor.h"
#include "sched.h"
#include "ecs.h"

void game_spawn_player(game_t* game)
{
    int x;
    int y;
    map_get_player_spawn(game->map, &x, &y);
    Entity player = make_actor(PLAYER, x, y, 100);
    game->action_list = sched_init(player);
}

void game_spawn_monsters(game_t* game)
{
    int x;
    int y;
    Entity p = sched_get_player(game->action_list);
    Position* player_pos = ecs_get_position(p);

    for (int i = 0; i < 50; i++) {
        map_get_npc_spawn(game->map, &x, &y, player_pos->x, player_pos->y);
        sched_add(game->action_list, make_actor(MONSTER, x, y, 80));
    }
}

void game_spawn_npc(game_t* game)
{
    int x;
    int y;
    Entity p = sched_get_player(game->action_list);
    Position* player_pos = ecs_get_position(p);

    for (int i = 0; i < 50; i++) {
        map_get_npc_spawn(game->map, &x, &y, player_pos->x, player_pos->y);
        sched_add(game->action_list, make_actor(NPC, x, y, 80));
    }
}

void game_debug_dump(game_t* game)
{
    FILE *fp_map = fopen("debug_map.txt", "w");
    if (fp_map) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                fputc(game->map->map[y][x], fp_map);
            }
            fputc('\n', fp_map);
        }
        fclose(fp_map);
    }

    FILE* f = fopen("debug_actor_list.txt", "w");
    if (!f) return;

    if (!game->action_list) {
        fprintf(f, "(empty scheduler list)\n");
        fclose(f);
        return;
    }

    sched_node* cur = game->action_list;
    fprintf(f, "=== Scheduler Dump ===\n");

    do {
        Entity e = cur->entity;
        Position* pos = ecs_get_position(e);
        Health* health = ecs_get_health(e);
        Combat* combat = ecs_get_combat(e);
        Energy* energy = ecs_get_energy(e);
        Renderable* render = ecs_get_renderable(e);
        actor_type type = ecs_get_actor_type(e);

        fprintf(f,
                "Node %p | prev=%p next=%p\n"
                "  Entity ID: %u\n"
                "  Type: %d\n"
                "  Glyph: %c\n"
                "  Pos: (%d, %d)\n"
                "  Speed: %d\n"
                "  Energy: %d\n"
                "  Alive: %s\n"
                "  HP: %d/%d\n"
                "  DMG: %d\n\n",
                (void*)cur,
                (void*)cur->prev,
                (void*)cur->next,
                (unsigned)e,
                type,
                render ? render->glyph : '?',
                pos ? pos->x : -1,
                pos ? pos->y : -1,
                energy ? energy->speed : 0,
                energy ? energy->energy : 0,
                health && health->alive ? "yes" : "no",
                health ? health->hp : 0,
                health ? health->max_hp : 0,
                combat ? combat->damage : 0
        );

        cur = cur->next;
    } while (cur != game->action_list);

    fprintf(f, "=== End of List ===\n");
    fclose(f);
}

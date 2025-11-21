#include <stdlib.h>
#include <stdio.h>


#include "game.h"
#include "map.h"

void game_spawn_player(game_t* game)
{
    int x;
    int y;
    map_get_player_spawn(game->map, &x, &y);
    game->action_list = sched_init(make_actor(PLAYER, x,y,10, player_act, game));
}

void game_spawn_monsters(game_t* game)
{
    int x;
    int y;
    actor_t* p = sched_get_player(game->action_list);
    for(int i = 0; i < 50; i++)
    {
        map_get_npc_spawn(game->map, &x, &y, p->x, p->y);
        sched_add(game->action_list, make_actor(MONSTER,x,y,10,monster_act,game));
    }
}

void game_spawn_npc(game_t* game)
{
    int x;
    int y;
    actor_t* p = sched_get_player(game->action_list);
    for(int i = 0; i < 50; i++)
    {
        map_get_npc_spawn(game->map, &x, &y, p->x, p->y);
        sched_add(game->action_list, make_actor(NPC,x,y,10,monster_act,game));
    }
}

void game_debug_dump(game_t* game)
{
    FILE *fp_map = fopen("debug_map.txt", "w");
    for(int x = 0; x < MAP_WIDTH; x++)
    {
        for(int y = 0; y < MAP_HEIGHT; y++)
        {
            
            fputc(game->map->map[y][x],fp_map);
        }
        fputc('\n',fp_map);
    }
    fclose(fp_map);
    #include <stdio.h>


    FILE* f = fopen("debug_actor_list.txt", "w");

    if (!game->action_list) {
        fprintf(f, "(empty scheduler list)\n");
        fclose(f);
        return;
    }

    sched_node* cur = game->action_list;
    fprintf(f, "=== Scheduler Dump ===\n");

    do {
        actor_t* a = cur->entity;

        fprintf(f,
                "Node %p | prev=%p next=%p\n"
                "  Actor ID: %d\n"
                "  Type: %d\n"
                "  Char: %c\n"
                "  Pos: (%d, %d)\n"
                "  Speed: %d\n"
                "  Alive: %s\n\n",
                (void*)cur,
                (void*)cur->prev,
                (void*)cur->next,
                a->id,
                a->type,
                a->ascii_char,
                a->x, a->y,
                a->speed,
                a->isAlive ? "yes" : "no"
        );

        cur = cur->next;

    } while (cur != game->action_list);

    fprintf(f, "=== End of List ===\n");

    fclose(f);



}
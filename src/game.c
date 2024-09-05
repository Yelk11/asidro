#include <ncurses.h>
#include <stdlib.h>

#include "map.h"
#include "entity.h"
#include "game.h"
#include "screen.h"
#include "linked_list.h"

void initializeGame(game_t* game)
{
    game->map = genMap();
}

void updateGame(game_t* game)
{
    int ch;
    position_t* start_pos = create_position(5,5);

    entity_t* player = createEntity(start_pos, '@');
    list_node* entity_list = list_create(player);
    game->map->entity_list = entity_list;
    nodelay(stdscr, true);
    while(getch() != 'q'){
        ch = getch();
        moveEntity(player, ch);
        updateMap(game->map);
        update_screen(game->map);
        
	}
    
}

void freeGame(game_t* game)
{
    freeMap(game->map);
}


void game(void)
{
    game_t* game = calloc(1, sizeof(game_t));
    initializeGame(game);
    updateGame(game);
    freeGame(game);
}


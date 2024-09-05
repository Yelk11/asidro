#include <stdlib.h>

#include "map.h"
#include "linked_list.h"



map_t* genMap(void)
{
    
    map_t* map = calloc(1, sizeof(map_t));
    for(int x = 0; x < MAP_WIDTH; x++)
    {
        for(int y = 0; y < MAP_HEIGHT; y++)
        {
            map->map[y][x] = '.';
            
        }
    }
    return map;
}

map_t* updateMap(map_t* map)
{
    map_t* new_map = calloc(1, sizeof(map_t));
    list_node* curr_node = map->entity_list;
    while(curr_node != NULL)
    {
        entity_t* e = (entity_t*) curr_node->data;
        new_map->map[e->pos->x][e->pos->y] = e->ascii_char;
        curr_node = curr_node->next;
    }
    return new_map;
}

void freeMap(map_t* map)
{

}

void add_entity_map(entity_t* entity)
{
    
}
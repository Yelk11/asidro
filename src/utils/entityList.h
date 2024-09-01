#include <stdlib.h>

#include "entity.h"


typedef struct node{
    node* next;
    entity* entity;
}node;


void insertAfter(node* list, entity* node);
void rem(node* list, entity* node);

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>




typedef struct list_t{
    struct list_t* next;
    void* value;
}list_t;



list_t* list_new(void* value);

void* list_value(list_t* list);

void list_insert(list_t* list, void* value);

void list_remove(list_t* list, void* value);

#endif
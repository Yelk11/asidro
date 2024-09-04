
#include "linked_list.h"

list_t* list_new(void* value)
{
    list_t* list = calloc(1, sizeof(list_t));
    list->value = value;
}



void* list_value(list_t* list)
{
	return list->value;
}

list_t* list_insert(list_t* list, void* value)
{
	list_t* new_node = list_new(value);
	new_node->next = list;
	return new_node;
}

void list_remove(list_t* list, void* value)
{
	list_t* cur = list;
	list_t* last_node;
	last_node = list;
	while(cur != NULL)
	{
		if(cur->value = value)
		{
			last_node->next = cur->next;
			free(cur);
		}else{
			last_node = cur;
			cur = cur->next;
		}
	}
}
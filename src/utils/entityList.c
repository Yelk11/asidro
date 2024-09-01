
#include "entityList.h"



node* createNode(entity* entity)
{
    node* newNode = calloc(1, sizeof(node));
    newNode->entity = entity;
    newNode->next = NULL;
    return newNode;
}

void insertAfter(node* root, entity* entity)
{
    while(root->next != NULL)
    {
        root = root->next;
    }
    root->next = createNode(entity);
}

void rem(node* list, entity* node)
{

}










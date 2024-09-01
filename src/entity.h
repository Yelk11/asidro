#ifndef ENTITY_H
#define ENTITY_H


typedef struct position{
    int x;
    int y;
}position;

typedef struct entity{
    position pos;
    char ascii_char;
}entity;


entity* createEntity(position pos, char charector);
void moveEntity(entity* e, char dir);



#endif
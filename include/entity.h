#ifndef ENTITY_H
#define ENTITY_H


typedef struct position_t{
    int x;
    int y;
}position_t;

typedef struct entity_t{
    struct position_t* pos;
    char ascii_char;
}entity_t;

typedef enum {
    UP,
    DOWN,
    RIGHT,
    LEFT
}direction;

position_t* create_position(int x, int y);
entity_t* createEntity(position_t* pos, char charector);
void moveEntity(entity_t* e, char dir);



#endif
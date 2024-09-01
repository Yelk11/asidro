
#define MAP_HEIGHT 500
#define MAP_WIDTH 500

typedef struct map{
    char map[MAP_HEIGHT][MAP_WIDTH];
}map;


void updateMap(void);
void genMap(void);
void freeMap(void);






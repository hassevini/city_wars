#ifndef __ENEMY__
#define __ENEMY__

#include "gun.h"

typedef struct enemy{
    unsigned short width;
    unsigned short height;
    unsigned short x;
    unsigned short y;
    unsigned short hp;
    unsigned char face;
} enemy;

enemy* enemy_create(unsigned short width, unsigned short height, unsigned short x, unsigned short y);
void enemy_move();
void enemy_destroy(enemy*);

#endif
#ifndef __ENEMY__
#define __ENEMY__

#include "gun.h"

typedef struct{
    unsigned short width;
    unsigned short height;
    unsigned short x;
    unsigned short y;
    unsigned short hp;
    unsigned char face;
} enemy;

typedef struct {
    enemy *stats;
    gun *rifle;
    
} raider;

typedef struct{
    enemy *stats;
    gun *rifle;
    unsigned char stage;
} boss;

enemy* enemy_create(unsigned short width, unsigned short height, unsigned short x, unsigned short y);
void enemy_move(enemy *element);
void enemy_destroy(enemy* element);

raider *raider_create(unsigned short x, unsigned short y);
void destroy_raider(raider *element);

boss *boss_create(unsigned short x, unsigned short y);
void boss_destroy(boss *element);

#endif
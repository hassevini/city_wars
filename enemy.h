#ifndef __ENEMY__
#define __ENEMY__

#include "gun.h"

#define RAIDER_WIDTH 40
#define RAIDER_HEIGHT 80
#define RAIDER_HP 5

#define BOSS_WIDTH 80
#define BOSS_HEIGHT 160
#define BOSS_HP 30

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

enemy* enemy_create(unsigned short width, unsigned short height, unsigned short x, unsigned short y, unsigned short hp, unsigned char face);
void enemy_move(enemy *element);
void enemy_destroy(enemy* element);

raider *raider_create(unsigned short x, unsigned short y, unsigned char face);
void raider_destroy(raider *element);

boss *boss_create(unsigned short x, unsigned short y, unsigned char face);
void boss_destroy(boss *element);

#endif
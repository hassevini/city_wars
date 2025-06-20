#ifndef __ENEMY__
#define __ENEMY__

#include "gun.h"
#include "character.h"

#define RAIDER_WIDTH 60
#define RAIDER_HEIGHT 140
#define RAIDER_HP 3
#define RAIDER_MOVE 5
#define RAIDER_BULLET_MOVE 10
#define RAIDER_SHOT_DELAY 30

#define BOSS_WIDTH 80
#define BOSS_HEIGHT 160
#define BOSS_HP 30
#define BOSS_SHOT_DELAY 30

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
    unsigned short x0;
    unsigned short y0;
    unsigned short walk_range;
    unsigned short shot_timer;
    gun *rifle;   
} raider;

typedef struct{
    enemy *stats;
    gun *rifle;
    unsigned short shot_timer;
    unsigned char stage;
} boss;

enemy* enemy_create(unsigned short width, unsigned short height, unsigned short x, unsigned short y, unsigned short hp, unsigned char face);
void enemy_destroy(enemy* element);

raider *raider_create(unsigned short x, unsigned short y, unsigned short walk_range, unsigned char face);
int raider_on_screen(raider *raider, unsigned short min_x, unsigned short max_x);
int raider_detect_character(raider *element, character *player);
void raider_move(raider *element);
void raider_shot(raider *element);
int raider_hit_bullet(raider *element, bullet *shot);
void raider_update_bullets(raider *element, unsigned short min_x, unsigned short max_x);
void raider_destroy(raider *element);

boss *boss_create(unsigned short x, unsigned short y, unsigned char face);
void boss_shot(boss *element);
int boss_hit_bullet(boss *element, bullet *shot);
void boss_update_bullets(boss *element, unsigned short min_x, unsigned short max_x);
void boss_destroy(boss *element);

#endif
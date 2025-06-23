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

#define BOSS_WIDTH 120
#define BOSS_HEIGHT 280
#define BOSS_HP 30
#define BOSS_SHOT_DELAY 30
#define BOSS_SPRITE_DELAY 10
#define BOSS_N_STATES 2

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
    unsigned short sprite;
    gun *rifle;   
} raider;

typedef struct{
    enemy *stats;
    gun *rifle;
    unsigned short shot_timer;
    unsigned short sprite_timer;
    unsigned char stage;
    unsigned short sprite;
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
int raiders_bullets_collision(character *player, raider **raiders, unsigned short n_raiders, unsigned short damage);
void player_bullets_collision_raiders(character *player, raider **raiders, unsigned short n_raiders);
void raider_destroy(raider *element);

boss *boss_create(unsigned short x, unsigned short y, unsigned char face);
int boss_on_screen(boss *element, unsigned short min_x, unsigned short max_x);
int boss_detect_character(boss *element, character *player);
void boss_move(boss *element);
void boss_shot(boss *element);
int boss_collision(boss *element, character *player);
int boss_hit_bullet(boss *element, bullet *shot);
void boss_update_bullets(boss *element, unsigned short min_x, unsigned short max_x);
int boss_bullets_collision(character *player, boss *final_boss, unsigned short damage);
int player_bullets_collision_boss(character *player, boss *final_boss);
void boss_destroy(boss *element);

#endif
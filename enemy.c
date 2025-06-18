#include <stdlib.h>
#include "enemy.h"


enemy* enemy_create(unsigned short width, unsigned short height, unsigned short x, unsigned short y, unsigned short hp, unsigned char face){
    enemy *new_enemy = (enemy*) malloc(sizeof(enemy));

    if(!new_enemy)
        return NULL;

    new_enemy->width = width;
    new_enemy->height = height;
    new_enemy->x = x;
    new_enemy->y = y;
    new_enemy->hp = hp;
    new_enemy->face = face;
}

void enemy_destroy(enemy *element){
    free(element);
}

raider *raider_create(unsigned short x, unsigned short y, unsigned char face){
    raider *new_raider = (raider*) malloc(sizeof(raider));
    
    if(!new_raider)
        return NULL;
    
    new_raider->stats = (enemy*) enemy_create(RAIDER_WIDTH, RAIDER_HEIGHT, x, y, RAIDER_HP, face); 
    return new_raider;
}

void raider_destroy(raider *element){
    enemy_destroy(element->stats);
    gun_destroy(element->rifle);
    free(element);
}

boss *boss_create(unsigned short x, unsigned short y, unsigned char face){
    boss *new_boss = (boss*) malloc(sizeof(boss));
    
    if(!new_boss)
        return NULL;
    
    new_boss->stats = (boss*) enemy_create(BOSS_WIDTH, BOSS_HEIGHT, x, y, BOSS_HP, face); 
    return new_boss;
}

void boss_destroy(boss *element){
    enemy_destroy(element->stats);
    gun_destroy(element->rifle);
    free(element);
}
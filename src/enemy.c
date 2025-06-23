#include <stdlib.h>
#include <stdio.h>
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

    return new_enemy;
}

void enemy_destroy(enemy *element){
    free(element);
}

raider *raider_create(unsigned short x, unsigned short y, unsigned short walk_range, unsigned char face){
    raider *new_raider = (raider*) malloc(sizeof(raider));
    
    if(!new_raider)
        return NULL;

    new_raider->x0 = x;
    new_raider->y0 = y;
    new_raider->walk_range = walk_range;
    new_raider->shot_timer = 0;
    new_raider->sprite = 0;

    new_raider->stats = (enemy*) enemy_create(RAIDER_WIDTH, RAIDER_HEIGHT, x, y, RAIDER_HP, face);
    new_raider->rifle = (gun*) gun_create();

    return new_raider;
}

int raider_on_screen(raider *element, unsigned short min_x, unsigned short max_x){
    if(element->stats->x + element->stats->width/2 > min_x)
        return 1;
    if(element->stats->x - element->stats->width/2 < max_x)
        return 1;
    
    return 0;
}

int raider_detect_character(raider *element, character *player){
    if((element->stats->y <= player->y + player->height/2) && (element->stats->y >= player->y - player->height/2)){
        if(element->stats->face && element->stats->x < player->x)
            return 1;
        if(!element->stats->face && element->stats->x > player->x)
            return 1;
    }

    return 0;
}

void raider_move(raider *element){
    if(element->stats->face){
        if(element->stats->x < element->x0 + element->walk_range){
            element->stats->x += RAIDER_MOVE;
        }
        else{
            element->stats->face = 0;
            element->stats->x = element->x0 + element->walk_range;
        }
    }
    else{
        if(element->stats->x > element->x0 - element->walk_range){
            element->stats->x -= RAIDER_MOVE;
        }
        else{
            element->stats->face = 1;
            element->stats->x = element->x0 - element->walk_range;
        }
    }
}

void raider_shot(raider *element){
    bullet *shot;
    // atira para a esquerda
    if(!element->stats->face)
        shot = gun_shot(element->stats->x - element->stats->width/2, element->stats->y, element->stats->face, element->rifle);
    // atira para a direita
    else if(element->stats->face == 1)
        shot = gun_shot(element->stats->x + element->stats->width/2, element->stats->y, element->stats->face, element->rifle);
    
    if(shot){
        element->rifle->shots = shot;
        element->shot_timer = RAIDER_SHOT_DELAY;
    }
}

int raider_hit_bullet(raider *element, bullet *shot){
    return (shot->x > element->stats->x - element->stats->width/2) && (shot->x < element->stats->x + element->stats->width/2) && 
    (shot->y > element->stats->y - element->stats->height/2) && (shot->y < element->stats->y + element->stats->height/2);
}

void raider_update_bullets(raider *element, unsigned short min_x, unsigned short max_x){
    bullet *index = element->rifle->shots;
    bullet *previous = NULL;

    while(index){
        bullet *next = (bullet*) index->next;

        if(!index->trajectory)
            index->x -= RAIDER_BULLET_MOVE;
        else if(index->trajectory == 1)
            index->x += RAIDER_BULLET_MOVE;

        if((index->x < min_x) || (index->x > max_x)){
            if(previous)
                previous->next = (bullet*) next;
            else
                element->rifle->shots = next;

            bullet_destroy(index);
        } 
        else 
            previous = index;        

        index = next;
    }
}

int raiders_bullets_collision(character *player, raider **raiders, unsigned short n_raiders, unsigned short damage){
    if(!player->hp)
        return 0;

    for(int i = 0; i < n_raiders; i++){
        if(!raiders[i])
            continue;

        bullet *current = raiders[i]->rifle->shots;
        bullet *previous = NULL;

        while(current){
            bullet *next = (bullet*) current->next;

            if(character_hit_bullet(player, current)){
                if(player->hp <= damage){
                    player->hp = 0;
                    return 0;
                }

                player->hp -= damage;

                // Remover bala
                if(previous)
                    previous->next = next;
                else
                    raiders[i]->rifle->shots = next;

                bullet_destroy(current);
            } 
            else{
                previous = current;
            }

            current = next;
        }
    }
    return 1;
}

void player_bullets_collision_raiders(character *player, raider **raiders, unsigned short n_raiders){
    bullet *index = player->rifle->shots;
    bullet *next;

    while(index){
        next = (bullet*) index->next;                

        for(int i = 0; i < n_raiders; i++){
            if(!raiders[i]->stats->hp)
                continue;
                    
            if(raider_hit_bullet(raiders[i], index)){
                if(raiders[i]->stats->hp <= 1)
                    raiders[i]->stats->hp = 0;
                else
                    raiders[i]->stats->hp--;

                gun_bullet_remove(player->rifle, index);
                break;
            }
        }
        index = next;
    }
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
    
    new_boss->stats = (enemy*) enemy_create(BOSS_WIDTH, BOSS_HEIGHT, x, y, BOSS_HP, face);
    new_boss->rifle = (gun*) gun_create();
    new_boss->stage = 0;
    new_boss->sprite = 0;
    new_boss->shot_timer = BOSS_SHOT_DELAY;
    new_boss->sprite_timer = 0;

    return new_boss;
}

int boss_on_screen(boss *element, unsigned short min_x, unsigned short max_x){
    if(element->stats->x + element->stats->width/2 > min_x)
        return 1;
    if(element->stats->x - element->stats->width/2 < max_x)
        return 1;
    
    return 0;
}

int boss_detect_character(boss *element, character *player){
    if((element->stats->y <= player->y + player->height/2) && (element->stats->y >= player->y - player->height/2)){
        if(element->stats->face && element->stats->x < player->x)
            return 1;
        if(!element->stats->face && element->stats->x > player->x)
            return 1;
    }

    return 0;
}

void boss_move(boss *element){
    if(element->stage)
        element->stats->x += 2;
    else
        element->stats->x -= 2;
}

void boss_shot(boss *element){
    bullet *shot1, *shot2;

    if(element->stage == 0){
        if(!element->stats->face){
            shot1 = gun_shot(element->stats->x - element->stats->width/2, element->stats->y, element->stats->face, element->rifle);
            shot2 = gun_shot(element->stats->x - element->stats->width/2, element->stats->y + element->stats->y/8, element->stats->face, element->rifle);
        }
        else if(element->stats->face == 1){
            shot1 = gun_shot(element->stats->x + element->stats->width/2, element->stats->y, element->stats->face, element->rifle);
            shot2 = gun_shot(element->stats->x + element->stats->width/2, element->stats->y + element->stats->y/8, element->stats->face, element->rifle);
        }
    }
    else if(element->stage == 1){
        if(!element->stats->face){
            shot1 = gun_shot(element->stats->x - element->stats->width/2, element->stats->y, element->stats->face, element->rifle);
            shot2 = gun_shot(element->stats->x - element->stats->width/2, element->stats->y - element->stats->y/8, element->stats->face, element->rifle);
        }
        else if(element->stats->face == 1){
            shot1 = gun_shot(element->stats->x + element->stats->width/2, element->stats->y, element->stats->face, element->rifle);
            shot2 = gun_shot(element->stats->x + element->stats->width/2, element->stats->y - element->stats->y/8, element->stats->face, element->rifle);
        }
    }

    if (shot2) {
        shot2->next = element->rifle->shots;
        element->rifle->shots = shot2;
    }

    if (shot1) {
        shot1->next = element->rifle->shots;
        element->rifle->shots = shot1;
    }

    element->shot_timer = RAIDER_SHOT_DELAY;
}

int boss_collision(boss *element, character *player) {
    // limites do boss
    unsigned short boss_left = element->stats->x - element->stats->width / 2;
    unsigned short boss_right = element->stats->x + element->stats->width / 2;
    unsigned short boss_top = element->stats->y - element->stats->height / 2;
    unsigned short boss_bottom = element->stats->y + element->stats->height / 2;

    // limites do player
    unsigned short player_left = player->x - player->width / 2;
    unsigned short player_right = player->x + player->width / 2;
    unsigned short player_top = player->y - player->height / 2;
    unsigned short player_bottom = player->y + player->height / 2;

    // Verifica colisão
    if(boss_right > player_left && boss_left < player_right && boss_bottom > player_top && boss_top < player_bottom){
        return 1;
    }

    return 0;
}

int boss_hit_bullet(boss *element, bullet *shot){
    return (shot->x > element->stats->x - element->stats->width/2) && (shot->x < element->stats->x + element->stats->width/2) && 
    (shot->y > element->stats->y - element->stats->height/2) && (shot->y < element->stats->y + element->stats->height/2);
}

void boss_update_bullets(boss *element, unsigned short min_x, unsigned short max_x){
    bullet *index = element->rifle->shots;
    bullet *previous = NULL;

    while(index){
        bullet *next = (bullet*) index->next;

        if(!index->trajectory)
            index->x -= RAIDER_BULLET_MOVE;
        else if(index->trajectory == 1)
            index->x += RAIDER_BULLET_MOVE;

        if((index->x < min_x) || (index->x > max_x)){
            if(previous)
                previous->next = (bullet*) next;
            else
                element->rifle->shots = next;

            bullet_destroy(index);
        } 
        else 
            previous = index;        

        index = next;
    }
}

int boss_bullets_collision(character *player, boss *final_boss, unsigned short damage){
    if(!player->hp)
        return 0;

    bullet *index = final_boss->rifle->shots;
    bullet *next;

   while(index){
        next = (bullet*) index->next;                

        if(character_hit_bullet(player, index)) {
            if(player->hp <= damage){
                player->hp = 0;
                return 0;
            }

            player->hp -= damage;
            gun_bullet_remove(final_boss->rifle, index);
        }
        index = next;
    }
    return 1;
}

int player_bullets_collision_boss(character *player, boss *final_boss){
    bullet *index = player->rifle->shots;
    bullet *next;

    while(index){
        next = (bullet*) index->next;                
                    
        if(boss_hit_bullet(final_boss, index)){
            if(final_boss->stats->hp == 1){
                final_boss->stats->hp--;
                return 1;
            }

            final_boss->stats->hp--;
            gun_bullet_remove(player->rifle, index);
        }
        index = next;
    }

    return 0;
}

void boss_destroy(boss *element){
    enemy_destroy(element->stats);
    gun_destroy(element->rifle);
    free(element);
}
#include <stdlib.h>
#include "character.h"

character *character_create(unsigned short width, unsigned short height, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, unsigned char face){
    if((x - width/2 < 0) || (x + width/2 > max_x) || (y - height/2 < 0) || (y + height/2 > max_y))
        return NULL;

    character *new_character = (character*) malloc(sizeof(character));

    if(!new_character)
        return NULL;

    new_character->height = height;
    new_character->width = width;
    new_character->x = x;
    new_character->y = y;
    new_character->ground = 1;
    new_character->velocity_y = 0;
    new_character->sprite_frame = 0;
    new_character->sprite_frame_max = 7;
    new_character->sprite_timer = 0;
    new_character->face = face;
    new_character->control = joystick_create();
    new_character->rifle = gun_create();

    return new_character;
}

void character_move(character *element, unsigned short steps, unsigned short trajectory, unsigned short max_x, unsigned short max_y){
    // movimento para a esquerda
    if(trajectory == 0){
        if((element->x - steps*CHARACTER_STEP) - element->width/2 >= 0)
            element->x -= steps*CHARACTER_STEP;
    }
    // movimento para a direita
    else if(trajectory == 1){
        if((element->x + steps*CHARACTER_STEP) + element->width/2 <= max_x)
            element->x += steps*CHARACTER_STEP;
    }
    // movimento para cima
    else if(trajectory == 2){
        if((element->y - steps*CHARACTER_STEP) - element->height/2 >= 0)
            element->y -= steps*CHARACTER_STEP;
    }
    // movimento para baixo
    else if (trajectory == 3) {
        if ((element->y + steps*CHARACTER_STEP) + element->height/2 < max_y) {
            element->y += steps*CHARACTER_STEP;
            element->ground = 0;
        } else {
            element->y = max_y - element->height / 2;
            element->velocity_y = 0;
            element->ground = 1;
        }
    }
}

void character_update_bullets(character *player, unsigned short min_x, unsigned short max_x){
    bullet *index = player->rifle->shots;
    bullet *previous = NULL;

    while(index){
        bullet *next = (bullet*) index->next;

        if(!index->trajectory)
            index->x -= BULLET_MOVE;
        else if(index->trajectory == 1)
            index->x += BULLET_MOVE;

        if((index->x < min_x) || (index->x > max_x)){
            if(previous)
                previous->next = (bullet*) next;
            else
                player->rifle->shots = next;

            bullet_destroy(index);
        } 
        else 
            previous = index;        

        index = next;
    }
}

void character_update_position(character *player, unsigned short max_x, unsigned short max_y){
    // movimento para a esquerda
    if(player->control->left){
        character_move(player, 6, 0, max_x, max_y);
        player->face = 0;
    }
    // movimento para a direita
    if(player->control->right){
        character_move(player, 6, 1, max_x, max_y);
        player->face = 1;
    }
    // movimento de salto
    if(player->control->up && player->ground){
        player->velocity_y = -15;
        player->ground = 0;
    }
    // movimento de queda
    if(!player->ground){
        player->velocity_y += 1;
        if(player->velocity_y > 10)
            player->velocity_y = 10;
        
        if(player->velocity_y < 0)
            character_move(player, -player->velocity_y, 2, max_x, max_y);
        else
            character_move(player, player->velocity_y, 3, max_x, max_y);
    }
    // atirar
    if(player->control->fire && !player->rifle->timer){
        character_shot(player);
        player->rifle->timer = PISTOL_COOLDOWN;
    }
}

void character_shot(character *element){
    bullet *shot;
    // atira para a esquerda
    if(!element->face)
        shot = gun_shot(element->x - element->width/2, element->y, element->face, element->rifle);
    // atira para a direita
    else if(element->face == 1)
        shot = gun_shot(element->x + element->width/2, element->y, element->face, element->rifle);
    
    if(shot)
        element->rifle->shots = shot;
}

void character_destroy(character *element){
    joystick_destroy(element->control);
    gun_destroy(element->rifle);
    free(element);
}
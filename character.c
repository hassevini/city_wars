#include <stdlib.h>
#include "character.h"

character *character_create(unsigned short width, unsigned short height, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y){
    if((x - width/2 < 0) || (x + width/2 > max_x) || (y - height/2 < 0) || (y + height/2 > max_y))
        return NULL;

    character *new_character = (character*) malloc(sizeof(character));

    if(!new_character)
        return NULL;

    new_character->height = height;
    new_character->width = width;
    new_character->x = x;
    new_character->y = y;

    return new_character;
}

void character_move(character *element, unsigned short steps, unsigned short trajectory, unsigned short max_x, unsigned short max_y){
    if(trajectory == 0){
        if((element->x - steps*CHARACTER_STEP) - element->width/2 >= 0)
            element->x -= steps*CHARACTER_STEP;
    }
    else if(trajectory == 1){
        if((element->x + steps*CHARACTER_STEP) + element->width/2 <= max_x)
            element->x += steps*CHARACTER_STEP;
    }
    else if(trajectory == 2){
        if((element->y - steps*CHARACTER_STEP) - element->height/2 >= 0)
            element->y -= steps*CHARACTER_STEP;
    }
    else if(trajectory == 3){
        if((element->y + steps*CHARACTER_STEP) + element->height/2 <= max_y)
            element->y += steps*CHARACTER_STEP;
    }
}

void character_destroy(character *element){
    free(element);
}
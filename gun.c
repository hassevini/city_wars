#include <stdlib.h>
#include "gun.h"

gun *gun_create(){
    gun *new_gun = (gun*) malloc(sizeof(gun));
    
    if(!new_gun)
        return NULL;

    new_gun->timer = 0;
    new_gun->shots = NULL;

    return new_gun;
}

bullet *gun_shot(unsigned short x, unsigned short y, unsigned char trajectory, gun *element){
    bullet *new_bullet = bullet_create(x, y, trajectory, element->shots);
    
    if(!new_bullet)
        return NULL;

    return new_bullet;
}

void gun_destroy(gun *element){
    bullet *index = element->shots;
    bullet *next;

    while(index){
        next = (bullet*) index->next;
        bullet_destroy(index);
        index = next;
    }

    free(element);
}

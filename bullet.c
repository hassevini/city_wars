#include <stdlib.h>
#include "bullet.h"

bullet *bullet_create(unsigned short x, unsigned short y, unsigned char trajectory, bullet *next){
    if(trajectory < 0 || trajectory > 1)
        return NULL;

    bullet *new_bullet = (bullet*) malloc(sizeof(bullet));

    if(!new_bullet)
        return NULL;

    new_bullet->x = x;
    new_bullet->y = y;
    new_bullet->trajectory = trajectory;
    new_bullet->next = next;

    return new_bullet;
}

void bullet_move(bullet *elements){
    bullet *index = elements;

    while(!index){
        if(!index->trajectory)
            index->x = index->x - BULLET_MOVE;
        else
            index->x = index->x + BULLET_MOVE;
            
        index = index->next;
    }
}

void bullet_destroy(bullet *element){
    free(element);
}
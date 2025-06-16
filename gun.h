#ifndef __GUN__
#define __GUN__

#include "bullet.h"

#define PISTOL_COOLDOWN 20

typedef struct {
    unsigned char timer;
    bullet *shots;
} gun;

gun *gun_create();
bullet *gun_shot(unsigned short x, unsigned short y, unsigned char trajectory, gun *element);
void gun_destroy(gun *element);

#endif

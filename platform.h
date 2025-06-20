#ifndef __PLATFORM__
#define __PLATFORM__

#include "character.h"

typedef struct {
    unsigned short x;
    unsigned short y;
    unsigned short width;
    unsigned short height;
    unsigned short top;
} platform;

platform *platform_create(unsigned short x, unsigned short y, unsigned short width, unsigned short height);
int platform_colision(platform *object, character *player);
void platform_destroy(platform *element);

#endif
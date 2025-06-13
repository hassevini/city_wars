#ifndef __CHARACTER__
#define __CHARACTER__

#include "joystick.h"

#define CHARACTER_STEP 10

typedef struct {
    unsigned short height;
    unsigned short width;
    unsigned short x;
    unsigned short y;
    joystick *control;
} character;

character *character_create(unsigned short width, unsigned short height, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y);
void character_move(character *element, unsigned short steps, unsigned short trajectory, unsigned short max_x, unsigned short max_y);
void character_destroy(character *element);

#endif
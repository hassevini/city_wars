#ifndef __CHARACTER__
#define __CHARACTER__

#include "joystick.h"
#include "gun.h"

#define X_SCREEN 1200
#define Y_SCREEN 800
#define CHARACTER_STEP 2

typedef struct {
    unsigned short height;
    unsigned short width;
    unsigned short x;
    unsigned short y;
    short velocity_y;
    unsigned char ground;
    unsigned char face;
    joystick *control;
    gun *rifle;
} character;

character *character_create(unsigned short width, unsigned short height, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, unsigned char face);
void character_move(character *element, unsigned short steps, unsigned short trajectory, unsigned short max_x, unsigned short max_y);
void character_update_bullets(character *player);
void character_update_position(character *player, unsigned short max_x, unsigned short max_y);
void character_shot(character *element);
void character_destroy(character *element);

#endif
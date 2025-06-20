#ifndef __CHARACTER__
#define __CHARACTER__

#include "joystick.h"
#include "gun.h"

#define X_SCREEN 1200
#define Y_SCREEN 800
#define CHARACTER_STEP 2
#define CHARACTER_HP 5

typedef struct {
    unsigned short height;
    unsigned short width;
    unsigned short x;
    unsigned short y;
    unsigned short hp;
    unsigned short sprite_frame;
    unsigned short sprite_frame_max;
    unsigned short sprite_timer;
    short velocity_y;
    unsigned char ground;
    unsigned char face;
    joystick *control;
    gun *rifle;
} character;

character *character_create(unsigned short width, unsigned short height, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, unsigned char face);
void character_move(character *element, unsigned short steps, unsigned short trajectory, unsigned short max_x, unsigned short max_y);
void character_update_bullets(character *player, unsigned short min_x, unsigned short max_x);
void character_update_position(character *player, unsigned short max_x, unsigned short max_y);
void character_update_frame(character *player);
void character_shot(character *element);
int character_hit_bullet(character *player, bullet *shot);
void character_destroy(character *element);

#endif
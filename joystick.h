#ifndef __JOYSTCIK__
#define __JOYSTCIK__

typedef struct {
    unsigned char right;
    unsigned char left;
    unsigned char up;
    unsigned char down;
    unsigned char fire;
} joystick;

typedef struct {
    int key_right;
    int key_left;
    int key_jump;
    int key_fire;
} controls;

joystick *joystick_create();
void joystick_right(joystick *element);
void joystick_left(joystick *element);
void joystick_up(joystick *element);
void joystick_down(joystick *element);
void joystick_fire(joystick *element);
void joystick_destroy(joystick *element);

#endif
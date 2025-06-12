#ifndef __JOYSTCIK__
#define __JOYSTCIK__

typedef struct {
    unsigned short right;
    unsigned short left;
    unsigned short up;
    unsigned short down;
} joystick;

joystick *joystick_create();
void joystick_right(joystick *element);
void joystick_left(joystick *element);
void joystick_up(joystick *element);
void joystick_down(joystick *element);
void joystick_destroy(joystick *element);

#endif
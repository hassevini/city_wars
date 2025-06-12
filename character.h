#ifndef __CHARACTER__
#define __CHARACTER__

#define CHARACTER_STEP 20

typedef struct {
    unsigned short height;
    unsigned short width;
    unsigned short x;
    unsigned short y;
} character;

character *character_create(unsigned short width, unsigned short height, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y);
void character_move(character *element, unsigned short steps, unsigned short trajectory, unsigned short max_x, unsigned short max_y);
void character_destroy(character *element);

#endif
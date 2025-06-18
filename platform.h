#ifndef __PLATFORM__
#define __PLATFORM__

typedef struct {
    unsigned short x;
    unsigned short y;
    unsigned short width;
    unsigned short height;
} platform;

platform *platform_create(unsigned short x, unsigned short y, unsigned short width, unsigned short height);
void *platform_destroy(platform *element);

#endif
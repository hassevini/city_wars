#include <stdlib.h>
#include "platform.h"

platform *platform_create(unsigned short x, unsigned short y, unsigned short width, unsigned short height){
    platform *new_platform = (platform*) malloc(sizeof(platform));

    if(!new_platform)
        return NULL;
    
    new_platform->x = x;
    new_platform->y = y;
    new_platform->width = width;
    new_platform->height = height;

    return new_platform;
}

void platform_destroy(platform *element){
    free(element);
}
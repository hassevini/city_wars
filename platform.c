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
    new_platform->top = y - height/2;

    return new_platform;
}

int platform_colision(platform *plat, character *player) {
    float player_bottom = player->y + player->height / 2;

    // Verifica se o jogador está dentro da largura da plataforma
    if ((player->x <= plat->x + plat->width / 2) &&
        (player->x >= plat->x - plat->width / 2)) {

        unsigned short player_bottom_before = player_bottom - player->velocity_y;

        // Caso 1: jogador caiu na plataforma (atravessou o topo dela)
        if (player->velocity_y > 0 && player_bottom_before < plat->top + 10 && player_bottom >= plat->top) {
            player->y = plat->top - player->height / 2;
            player->velocity_y = 0;
            player->ground = 1;
            return 1;
        }

        // Caso 2: jogador já está em cima da plataforma, parado
        if (player->velocity_y == 0 && player_bottom == plat->top) {
            player->ground = 1;
            return 1;
        }
    }

    return 0;
}

void platform_destroy(platform *element){
    free(element);
}
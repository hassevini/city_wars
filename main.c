// Compilação: gcc main.c character.c joystick.c -o teste $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 --libs --cflags)

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include "character.h"
#include "joystick.h"

#define X_SCREEN 640
#define Y_SCREEN 640

int main(){
    al_init();
    al_init_primitives_addon();

    al_install_keyboard();

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_FONT *font = al_create_builtin_font();
    ALLEGRO_DISPLAY *disp = al_create_display(X_SCREEN, Y_SCREEN);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    character *player = character_create(20, 40, 10, Y_SCREEN-20, X_SCREEN, Y_SCREEN);
    if(!player)
        return 1;

    ALLEGRO_EVENT event;
    al_start_timer(timer);

    while(1){
        al_wait_for_event(queue, &event);

        if(event.type == ALLEGRO_EVENT_TIMER){
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_filled_rectangle(player->x - player->width/2, player->y - player->height/2, player->x + player->width/2, player->y + player->height/2, al_map_rgb(255, 0, 0));
            al_flip_display();
        }
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    character_destroy(player);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>

#include "character.h"
#include "joystick.h"

#define X_SCREEN 1200
#define Y_SCREEN 800

int main(){
    al_init();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    al_install_keyboard();

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_FONT *font = al_load_ttf_font("assets/font.ttf", 64, 0);
    ALLEGRO_DISPLAY *disp = al_create_display(X_SCREEN, Y_SCREEN);
    ALLEGRO_BITMAP *background = al_load_bitmap("assets/city1/10.png");

    int bg_width = al_get_bitmap_width(background);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    character *player = character_create(40, 80, 20, Y_SCREEN-40, X_SCREEN, Y_SCREEN, 1);

    if(!player)
        return 1;

    ALLEGRO_EVENT event;
    al_start_timer(timer);

    unsigned char menu = 1;
    unsigned char game = 1;

    // loop do menu
    while(menu){
        al_wait_for_event(queue, &event);

        if(event.type == ALLEGRO_EVENT_TIMER){
            al_clear_to_color(al_map_rgb(0, 0, 0));

            if(menu == 1){
                al_draw_text(font, al_map_rgb(255, 0, 0), X_SCREEN/2 - 80, Y_SCREEN/2 - 60, 0, "Iniciar Jogo");
                al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 80, Y_SCREEN/2 + 20, 0, "Sair");
            }
            else if(menu == 2){
                al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 80, Y_SCREEN/2 - 60, 0, "Iniciar Jogo");
                al_draw_text(font, al_map_rgb(255, 0, 0), X_SCREEN/2 - 80, Y_SCREEN/2 + 20, 0, "Sair");
            }
            
            al_flip_display();
        }
        else if(event.type == ALLEGRO_EVENT_KEY_DOWN){
            if(event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                menu = 2;
            else if(event.keyboard.keycode == ALLEGRO_KEY_UP)
                menu = 1;
            else if(event.keyboard.keycode == ALLEGRO_KEY_ENTER){
                if(menu == 2)
                    game = 0;
                menu = 0;
            }
        }
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            game = 0;
            menu = 0;
        }
    }

    unsigned int offset;

    // loop do jogo
    while(game){
        al_wait_for_event(queue, &event);

        // atualiza a tela
        if(event.type == ALLEGRO_EVENT_TIMER){
            offset++;

            if(offset >= bg_width)
                offset = 0;

            character_update_position(player, X_SCREEN, Y_SCREEN);
            al_draw_bitmap_region(background, offset, 400, X_SCREEN, Y_SCREEN, 0, 0, 0);
            al_draw_bitmap_region(background, 0, 400, offset, Y_SCREEN, bg_width - offset, 0, 0);
            al_draw_filled_rectangle(player->x - player->width/2, player->y - player->height/2, player->x + player->width/2, player->y + player->height/2, al_map_rgb(255, 0, 0));
            for(bullet *index = player->rifle->shots; index != NULL; index = (bullet*) index->next)
                al_draw_filled_circle(index->x, index->y, 4, al_map_rgb(255, 255, 0));
            if(player->rifle->timer)
                player->rifle->timer--;
            al_flip_display();
        }
        // detecta o uso das teclas
        else if((event.type == ALLEGRO_EVENT_KEY_DOWN) ||(event.type == ALLEGRO_EVENT_KEY_UP)){
            if(event.keyboard.keycode == ALLEGRO_KEY_UP)
                joystick_up(player->control);
            else if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                joystick_right(player->control);
            else if(event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                joystick_left(player->control);
            else if(event.keyboard.keycode == ALLEGRO_KEY_C)
                joystick_fire(player->control);
        }
        // encerra o jogo
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
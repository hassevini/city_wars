#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>

#include "character.h"
#include "joystick.h"
#include "platform.h"
#include "enemy.h"

#define X_SCREEN 1200
#define Y_SCREEN 800
#define MAX_X 12000



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
    ALLEGRO_BITMAP *background = al_load_bitmap("assets/city/10.png");
    ALLEGRO_BITMAP *player_sprite = al_load_bitmap("assets/player/Idle.png");

    int bg_width = al_get_bitmap_width(background);
    unsigned short camera_x;

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    character *player = character_create(60, 100, X_SCREEN/2, Y_SCREEN-50, X_SCREEN, Y_SCREEN, 1);

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

    unsigned short offset;

    // loop do jogo
    while(game){
        al_wait_for_event(queue, &event);

        // atualiza a tela
        if(event.type == ALLEGRO_EVENT_TIMER){

            // Limita a câmera para não sair dos limites do cenário
            if (player->x <= X_SCREEN / 2)
                camera_x = 0;
            else if (player->x >= MAX_X - X_SCREEN / 2)
                camera_x = MAX_X - X_SCREEN;
            else
                camera_x = player->x - X_SCREEN / 2;

            offset = camera_x % bg_width;

            character_update_position(player, MAX_X, Y_SCREEN);
            character_update_bullets(player, camera_x, camera_x + X_SCREEN);
            
            int first_region_width = bg_width - offset;
            if (first_region_width > X_SCREEN) 
                first_region_width = X_SCREEN;

            // Primeira parte da imagem (do offset até o fim da imagem)
            al_draw_bitmap_region(background, offset, 400, first_region_width, Y_SCREEN, 0, 0, 0);

            // Se necessário, desenha o restante da imagem a partir do começo
            if (first_region_width < X_SCREEN) {
                int remaining_width = X_SCREEN - first_region_width;
                al_draw_bitmap_region(background, 0, 400, remaining_width, Y_SCREEN, first_region_width, 0, 0);
            }

            unsigned short draw_x = player->x - camera_x;
            
            int frame_w = 110;
            int frame_h = 128;
            int sx = player->sprite_frame * frame_w;

            float scale = 1.5;
            int draw_w = frame_w * scale;
            int draw_h = frame_h * scale;

            al_draw_scaled_bitmap(player_sprite, sx+8, 30, frame_w, frame_h, draw_x - draw_w / 2, player->y - draw_h / 2, draw_w, draw_h, player->face ? 0 : ALLEGRO_FLIP_HORIZONTAL);

            for(bullet *index = player->rifle->shots; index != NULL; index = (bullet*) index->next){
                unsigned short bullet_draw_x = index->x - camera_x;
                al_draw_filled_circle(bullet_draw_x, index->y, 4, al_map_rgb(255, 255, 0));
            }

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
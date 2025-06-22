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
#define GROUND_HEIGHT 20
#define MAX_X 6000
#define N_PLATFORMS 6
#define N_RAIDERS 6
#define N_PLAYER_SPRITES 4
#define N_RAIDER_SPRITES 2
#define N_BOSS_SPRITES 2
#define BOSS_SCREEN_X (MAX_X - X_SCREEN)
#define BOSS_TRIGGER_MARGIN 200
#define BOSS_TRIGGER_ACTIVATION_X (BOSS_SCREEN_X + BOSS_TRIGGER_MARGIN)

ALLEGRO_BITMAP **init_player_sprites(){
    ALLEGRO_BITMAP **player_sprites = malloc(sizeof(ALLEGRO_BITMAP*)*N_PLAYER_SPRITES);

    if(!player_sprites)
        return NULL;
    
    player_sprites[0] = al_load_bitmap("assets/player/Idle.png");
    player_sprites[1] = al_load_bitmap("assets/player/Jump.png");
    player_sprites[2] = al_load_bitmap("assets/player/Walk.png");
    player_sprites[3] = al_load_bitmap("assets/player/Shot.png");

    return player_sprites;
}

ALLEGRO_BITMAP **init_raider_sprites(){
    ALLEGRO_BITMAP **raider_sprites = malloc(sizeof(ALLEGRO_BITMAP*)*N_RAIDER_SPRITES);

    if(!raider_sprites)
        return NULL;
    
    raider_sprites[0] = al_load_bitmap("assets/raider/Idle.png");
    raider_sprites[1] = al_load_bitmap("assets/raider/Shot.png");

    return raider_sprites;
}

ALLEGRO_BITMAP **init_boss_sprites(){
    ALLEGRO_BITMAP **boss_sprites = malloc(sizeof(ALLEGRO_BITMAP*)*N_BOSS_SPRITES);

    if(!boss_sprites)
        return NULL;
    
    boss_sprites[0] = al_load_bitmap("assets/boss/Idle.png");
    boss_sprites[1] = al_load_bitmap("assets/boss/Shot_1.png");

    return boss_sprites;
}

void character_update_sprite(character *player){
    if(player->control->fire)
        player->sprite = 3;
    else if(player->control->up || !player->ground)
        player->sprite = 1;
    else if(player->control->left || player->control->right)
        player->sprite = 2;
    else
        player->sprite = 0;
}

platform **init_platforms(){
    platform **platforms = malloc(sizeof(platform*) * N_PLATFORMS);

    if(!platforms) 
        return NULL;

    platforms[0] = platform_create(900, Y_SCREEN-170, 400, 20);
    platforms[1] = platform_create(2600, Y_SCREEN-320, 600, 20);
    platforms[2] = platform_create(2200, Y_SCREEN-170, 400, 20);
    platforms[3] = platform_create(3100, Y_SCREEN-170, 400, 20);
    platforms[4] = platform_create(4000, Y_SCREEN-170, 400, 20);
    platforms[5] = platform_create(4200, Y_SCREEN-320, 400, 20);

    return platforms;
}

raider **init_raiders(){
    raider **raiders = malloc(sizeof(raider*) * N_RAIDERS);

    if(!raiders)
        return NULL;
    
    raiders[0] = raider_create(900, Y_SCREEN - RAIDER_HEIGHT/2 - 182, 100, 0);
    raiders[1] = raider_create(2200, Y_SCREEN - RAIDER_HEIGHT/2 - 182, 100, 0);
    raiders[2] = raider_create(2300, Y_SCREEN - RAIDER_HEIGHT/2 - GROUND_HEIGHT, 100, 0);
    raiders[3] = raider_create(3100, Y_SCREEN - RAIDER_HEIGHT/2 - 182, 100, 0);
    raiders[4] = raider_create(4000, Y_SCREEN - RAIDER_HEIGHT/2 - 182, 100, 0);
    raiders[5] = raider_create(4200, Y_SCREEN - RAIDER_HEIGHT/2 - 322, 100, 0);

    return raiders;
}

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
    ALLEGRO_BITMAP **player_sprites = init_player_sprites();
    ALLEGRO_BITMAP **raider_sprites = init_raider_sprites();
    ALLEGRO_BITMAP **boss_sprites = init_boss_sprites();
    ALLEGRO_BITMAP *bullet_sprite = al_load_bitmap("assets/bullet.png");
    ALLEGRO_BITMAP *ground_tile = al_load_bitmap("assets/tile.png");
    ALLEGRO_BITMAP *platform_tile = al_load_bitmap("assets/platform.png");
    ALLEGRO_BITMAP *logo1 = al_load_bitmap("assets/city_logo.png");
    ALLEGRO_BITMAP *logo2 = al_load_bitmap("assets/wars_logo.png");

    int tile_w = al_get_bitmap_width(ground_tile);
    int bg_width = al_get_bitmap_width(background);
    short camera_x, draw_x, draw_y;

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    character *player = character_create(60, 140, X_SCREEN/2, Y_SCREEN-70-GROUND_HEIGHT, X_SCREEN, Y_SCREEN-20, 1);
    platform **platforms = init_platforms();
    raider **raiders = init_raiders();
    boss *final_boss = boss_create(MAX_X - 100, Y_SCREEN - GROUND_HEIGHT - BOSS_HEIGHT/2, 0);

    if(!player)
        return 1;

    ALLEGRO_EVENT event;
    al_start_timer(timer);

    unsigned char menu = 1, game = 1, win = 0, boss_fight = 0;
    unsigned short offset = 0;

    // loop do menu
    while(menu){
        al_wait_for_event(queue, &event);

        if(event.type == ALLEGRO_EVENT_TIMER){
            al_clear_to_color(al_map_rgb(0, 0, 0));

            int first_region_width = bg_width - offset;
            if (first_region_width > X_SCREEN) 
                first_region_width = X_SCREEN;

            al_draw_bitmap_region(background, offset, 0, first_region_width, Y_SCREEN, 0, 0, 0);
        
            if (first_region_width < X_SCREEN) {
                int remaining_width = X_SCREEN - first_region_width;
                al_draw_bitmap_region(background, 0, 0, remaining_width, Y_SCREEN, first_region_width, 0, 0);
            }

            al_draw_bitmap(logo1, 80, 10, 0);
            al_draw_bitmap(logo2, 540, 50, 0);

            if(menu == 1){
                al_draw_text(font, al_map_rgb(255, 165, 0), X_SCREEN/2 - 80, Y_SCREEN/2 + 20, 0, "Iniciar Jogo");
                al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 80, Y_SCREEN/2 + 100, 0, "Sair");
            }
            else if(menu == 2){
                al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 80, Y_SCREEN/2 + 20, 0, "Iniciar Jogo");
                al_draw_text(font, al_map_rgb(255, 165, 0), X_SCREEN/2 - 80, Y_SCREEN/2 + 100, 0, "Sair");
            }
            
            offset = (offset + 1)%bg_width;

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

    // loop do jogo
    while(game){
        al_wait_for_event(queue, &event);

        // atualiza a tela
        if(event.type == ALLEGRO_EVENT_TIMER){
            character_update_sprite(player);

            // Verifica se ainda há raiders vivos
            int raiders_vivos = 0;
            for(int i = 0; i < N_RAIDERS; i++){
                if(raiders[i]){
                    raiders_vivos++;
                }
            }

            // Ativa a boss fight se o jogador entrou na área após eliminar os raiders
            if (!boss_fight && !raiders_vivos && player->x >= BOSS_TRIGGER_ACTIVATION_X)
                boss_fight = 1;

            // Calcula a câmera
            if (boss_fight) {
                camera_x = BOSS_SCREEN_X;
            } else {
                if (player->x <= X_SCREEN / 2)
                    camera_x = 0;
                else if (player->x >= MAX_X - X_SCREEN / 2)
                    camera_x = MAX_X - X_SCREEN;
                else
                    camera_x = player->x - X_SCREEN / 2;
            }

            // Limita o movimento horizontal do jogador
            if (boss_fight) {
                if (player->x < BOSS_SCREEN_X + player->width / 2)
                    player->x = BOSS_SCREEN_X + player->width / 2;
                if (player->x > BOSS_SCREEN_X + X_SCREEN - player->width / 2)
                    player->x = BOSS_SCREEN_X + X_SCREEN - player->width / 2;
            } else {
                if (raiders_vivos && player->x > BOSS_TRIGGER_ACTIVATION_X)
                    player->x = BOSS_TRIGGER_ACTIVATION_X;
            }

            offset = camera_x % bg_width;
            
            // reseta antes de testar colisão
            player->ground = 0; 

            // Testa colisão com plataformas suspensas
            for (int i = 0; i < N_PLATFORMS; i++) {
                if (platform_colision(platforms[i], player)) {
                    break;
                }
            }

            // Verifica contato com o chão do mundo
            if (player->y + player->height / 2 >= Y_SCREEN-GROUND_HEIGHT) {
                player->y = Y_SCREEN - GROUND_HEIGHT - player->height / 2;
                player->velocity_y = 0;
                player->ground = 1;
            }

            character_update_position(player, MAX_X, Y_SCREEN-20);

            character_update_bullets(player, camera_x, camera_x + X_SCREEN);

            // Controla os raiders
            for(int i = 0; i < N_RAIDERS; i++){
                if(!raiders[i]) 
                    continue;

                if(raider_on_screen(raiders[i], camera_x, camera_x + X_SCREEN)){
                    if(raider_detect_character(raiders[i], player)){
                        if(!raiders[i]->shot_timer){
                            raider_shot(raiders[i]);
                            raiders[i]->sprite = 1;
                        }
                    }
                    else{
                        raider_move(raiders[i]);
                        raiders[i]->sprite = 0;
                    }
                }
                raider_update_bullets(raiders[i], camera_x, camera_x + X_SCREEN);
                
                if(raiders[i]->shot_timer)
                    raiders[i]->shot_timer--;
            }

            if(boss_fight){
                if(!final_boss->shot_timer){
                    boss_shot(final_boss);
                    final_boss->sprite = 1;
                    final_boss->stage = (final_boss->stage+1)%BOSS_N_STATES;
                    final_boss->sprite_timer = BOSS_SPRITE_DELAY;
                }
                else{
                    if(!final_boss->sprite_timer)
                        final_boss->sprite = 0;
                    else
                        final_boss->sprite_timer--;
                    final_boss->shot_timer--;
                }
            }

            boss_update_bullets(final_boss, camera_x, camera_x + X_SCREEN);

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

            for (int x = 0; x < X_SCREEN; x += tile_w) {
                al_draw_bitmap(ground_tile, x, Y_SCREEN - 20, 0);
            }

            al_draw_textf(font, al_map_rgb(255, 165, 0), 30, 20, 0, "HP: %d", player->hp);
            al_draw_textf(font, al_map_rgb(255, 165, 0), 400, 20, 0, "Inimigos Restantes: %d", raiders_vivos);

            draw_x = player->x - camera_x;
            
            int frame_w = 126;
            int frame_h = 128;
            int sx = player->sprite_frame * frame_w;

            float scale = 2.0;
            int draw_w = frame_w * scale;
            int draw_h = frame_h * scale;
            //al_draw_filled_rectangle(draw_x - player->width/2, player->y - player->height/2, draw_x + player->width/2, player->y + player->height/2, al_map_rgb(255, 0, 0));
            al_draw_scaled_bitmap(player_sprites[player->sprite], sx, 28, frame_w, frame_h, draw_x - draw_w / 2, player->y - draw_h / 2, draw_w, draw_h, player->face ? 0 : ALLEGRO_FLIP_HORIZONTAL);

            draw_x = final_boss->stats->x - final_boss->stats->width/2 - camera_x;
            draw_y = final_boss->stats->y - final_boss->stats->height/2;
            //al_draw_filled_rectangle(draw_x, draw_y, draw_x + final_boss->stats->width, draw_y + final_boss->stats->height, al_map_rgb(255, 0, 0));
            al_draw_scaled_bitmap(boss_sprites[final_boss->sprite], 230, 28, 110, frame_h, draw_x - 50, final_boss->stats->y - draw_h, 2*272, 2*draw_h, final_boss->stats->face ? 0 : ALLEGRO_FLIP_HORIZONTAL);

            // desenha as plataformas
            for (int i = 0; i < N_PLATFORMS; i++) {
                int start_x = platforms[i]->x - platforms[i]->width / 2 - camera_x;
                int start_y = platforms[i]->y - platforms[i]->height / 2;

                int tiles_horizontal = (platforms[i]->width + tile_w - 1) / tile_w;  // ceil para garantir cobertura completa

                for (int tx = 0; tx < tiles_horizontal; tx++) {
                    int draw_x = start_x + tx * tile_w;
                    int draw_y = start_y;

                    al_draw_bitmap(platform_tile, draw_x, draw_y, 0);
                }
            }     

            game = raiders_bullets_collision(player, raiders, N_RAIDERS);

            // desenha os raiders e suas balas
            for(int i = 0; i < N_RAIDERS; i++){
                if(!raiders[i])
                    continue;

                draw_x = raiders[i]->stats->x - raiders[i]->stats->width / 2 - camera_x;
                draw_y = raiders[i]->stats->y - raiders[i]->stats->height / 2;

                //al_draw_filled_rectangle(draw_x, draw_y, draw_x + raiders[i]->stats->width, draw_y + raiders[i]->stats->height, al_map_rgb(0, 255, 0));
                if(!raiders[i]->sprite)
                    al_draw_scaled_bitmap(raider_sprites[raiders[i]->sprite], 272, 28, 90, frame_h, draw_x - 100, raiders[i]->stats->y - draw_h / 2, 272, draw_h, raiders[i]->stats->face ? 0 : ALLEGRO_FLIP_HORIZONTAL);
                else
                    al_draw_scaled_bitmap(raider_sprites[raiders[i]->sprite], 240, 28, 90, frame_h, draw_x - 100, raiders[i]->stats->y - draw_h / 2, 272, draw_h, raiders[i]->stats->face ? 0 : ALLEGRO_FLIP_HORIZONTAL);

                for(bullet *index = raiders[i]->rifle->shots; index != NULL; index = index->next){
                    unsigned short bullet_draw_x = index->x - camera_x;
                    al_draw_scaled_bitmap(bullet_sprite, 350, 100, 60, 75, bullet_draw_x - 10, index->y - 10, 40, 20, index->trajectory ? 0 : ALLEGRO_FLIP_HORIZONTAL);
                }
            }

            boss_bullets_collision(player, final_boss);

            // desenha as balas do boss
            for(bullet *index = final_boss->rifle->shots; index != NULL; index = index->next){
                unsigned short bullet_draw_x = index->x - camera_x;
                al_draw_scaled_bitmap(bullet_sprite, 350, 100, 60, 75, bullet_draw_x - 10, index->y - 10, 40, 20, index->trajectory ? 0 : ALLEGRO_FLIP_HORIZONTAL);
            }
            
            player_bullets_collision_raiders(player, raiders, N_RAIDERS);
            win = player_bullets_collision_boss(player, final_boss);

            if(win){
                break;
            }
            
            // desenha as balas do player
            for(bullet *index = player->rifle->shots; index != NULL; index = index->next){
                unsigned short bullet_draw_x = index->x - camera_x;
                al_draw_scaled_bitmap(bullet_sprite, 350, 100, 60, 75, bullet_draw_x - 10, index->y - 10, 40, 20, index->trajectory ? 0 : ALLEGRO_FLIP_HORIZONTAL);
            }

            if(player->rifle->timer)
                player->rifle->timer--;
            al_flip_display();
        }
        // detecta o uso das teclas
        else if((event.type == ALLEGRO_EVENT_KEY_DOWN) || (event.type == ALLEGRO_EVENT_KEY_UP)){
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

    if(!player->hp || win){
        while(1){
            al_wait_for_event(queue, &event);

            if(event.type == ALLEGRO_EVENT_TIMER){
                al_clear_to_color(al_map_rgb(0, 0, 0));
                
                if(win){
                    al_draw_text(font, al_map_rgb(0, 255, 0), X_SCREEN/2 - 80, Y_SCREEN/2 - 180, 0, "GANHOU!");
                    al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 250, Y_SCREEN/2 + 40, 0, "Pressione ENTER para sair");
                }
                else{
                    al_draw_text(font, al_map_rgb(255, 0, 0), X_SCREEN/2 - 80, Y_SCREEN/2 - 180, 0, "GAME OVER");
                    al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 250, Y_SCREEN/2 + 40, 0, "Pressione ENTER para sair");
                }
                al_flip_display();
            }

            else if((event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ENTER) || event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                break;
            }
        }
    }

    for(int i = 0; i < N_PLATFORMS; i++)
        platform_destroy(platforms[i]);

    for(int i = 0; i < N_RAIDERS; i++)
        if(raiders[i])
            raider_destroy(raiders[i]);
    
    
    for(int i = 0; i < N_PLAYER_SPRITES; i++)
        al_destroy_bitmap(player_sprites[i]);

    for(int i = 0; i < N_RAIDER_SPRITES; i++)
        al_destroy_bitmap(raider_sprites[i]);

    for(int i = 0; i < N_BOSS_SPRITES; i++)
        al_destroy_bitmap(boss_sprites[i]);

    free(platforms);
    free(raiders);
    free(player_sprites);
    free(raider_sprites);
    free(boss_sprites);

    character_destroy(player);
    boss_destroy(final_boss);

    al_destroy_bitmap(background);
    al_destroy_bitmap(ground_tile);
    al_destroy_bitmap(platform_tile);
    al_destroy_bitmap(logo1);
    al_destroy_bitmap(logo2);

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
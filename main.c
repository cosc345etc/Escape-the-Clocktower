#include <allegro5/allegro5.h>
#include <stdio.h>
#include <stdlib.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include "Player.h"

enum MYKEYS {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

struct player
{
    int x;
    int y;
    char direction_facing;
};

typedef struct{
    ALLEGRO_BITMAP *N;
    ALLEGRO_BITMAP *E;
    ALLEGRO_BITMAP *S;
    ALLEGRO_BITMAP *W;
    bool passsable;
} MapTile;

MapTile map[4][1];

bool done;
bool redraw;
bool key[4] = { false, false, false, false };

ALLEGRO_EVENT_QUEUE* event_queue;
ALLEGRO_TIMER* timer;
ALLEGRO_DISPLAY* display;


player* player_constructer (void)
{
    struct player* instance = malloc (sizeof(player));
    if(instance != NULL) {
        instance->x = 0;
        instance->y = 0;
        instance->direction_facing = 'N';
    }
    return instance;
}

void player_set_position (player* this, int x, int y)
{
    this->x = x;
    this->y = y;
}

int player_get_position_x (const player* this)
{
    return this->x;
}

int player_get_position_y (const player* this)
{
    return this->y;
}

void player_set_direction(player* this, char d){
    this->direction_facing = d;
}

char player_get_direction_facing(const player* this){
    return this->direction_facing;
}

void player_destruct ( player* this)
{
    free(this);
}

player* p;

void init_map(void){
    map[0][0].N = al_load_bitmap("/Users/anaruhudson/CLionProjects/EtC-1`/assets/Example_corridor.JPG");
    map[0][0].E = al_load_bitmap("/Users/anaruhudson/CLionProjects/EtC-1/assets/Grumpy_bureaucrat_draft.JPG");
    map[0][0].S = al_load_bitmap("/Users/anaruhudson/CLionProjects/EtC-1/assets/Example_portrait.JPG");
    map[0][0].W = al_load_bitmap("/Users/anaruhudson/CLionProjects/EtC-1/assets/Grumpy_bureaucrat_draft.JPG");
    map[0][0].passsable = true;

    map[1][0].N = al_load_bitmap("/Users/anaruhudson/CLionProjects/EtC-1/assets/Example_textbox.JPG");
    map[1][0].E = al_load_bitmap("/Users/anaruhudson/CLionProjects/EtC-1/assets/Grumpy_bureaucrat_draft.JPG");
    map[1][0].S = al_load_bitmap("/Users/anaruhudson/CLionProjects/EtC-1/assets/Example_corridor.JPG");
    map[1][0].W = al_load_bitmap("/Users/anaruhudson/CLionProjects/EtC-1/assets/Grumpy_bureaucrat_draft.JPG");
    map[1][0].passsable = true;

    map[2][0].N = al_load_bitmap("/Users/anaruhudson/CLionProjects/EtC-1/assets/Example_corridor.JPG");
    map[2][0].E = al_load_bitmap("/Users/anaruhudson/CLionProjects/EtC-1/assets/Grumpy_bureaucrat_draft.JPG");
    map[2][0].S = al_load_bitmap("/Users/anaruhudson/CLionProjects/EtC-1/assets/Example_textbox.JPG");
    map[2][0].W = al_load_bitmap("/Users/anaruhudson/CLionProjects/EtC-1/assets/Grumpy_bureaucrat_draft.JPG");
    map[2][0].passsable = true;

    map[3][0].N = al_load_bitmap("/Users/anaruhudson/CLionProjects/EtC-1/assets/Example_portrait.JPG");
    map[3][0].E = al_load_bitmap("/Users/anaruhudson/CLionProjects/EtC-1/assets/Grumpy_bureaucrat_draft.JPG");
    map[3][0].S = al_load_bitmap("/Users/anaruhudson/CLionProjects/EtC-1/assets/Example_corridor.JPG");
    map[3][0].W = al_load_bitmap("/Users/anaruhudson/CLionProjects/EtC-1/assets/Grumpy_bureaucrat_draft.JPG");
    map[3][0].passsable = true;
}

void init_player(void){
    p = player_constructer();
}

void get_user_input(ALLEGRO_EVENT event){
    if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
                if(player_get_direction_facing(p) == 'N' && map[player_get_position_x(p) + 1][player_get_position_y(p)].passsable){
                    player_set_position(p, p->x+1, p->y);
                    key[KEY_UP] = true;
                    printf("Key up pressed\n");
                    redraw = true;
                }else if(player_get_direction_facing(p) == 'S' && map[player_get_position_x(p) - 1][player_get_position_y(p)].passsable){
                    player_set_position(p, p->x-1, p->y);
                    key[KEY_UP] = true;
                    printf("Key up pressed\n");
                    redraw = true;
                }else{
                    printf("Key up pressed, but can't advance..\n");
                }
                break;

//            case ALLEGRO_KEY_DOWN:
//                if(player_get_direction_facing(p) == 'N'){
//                    player_set_direction(p, 'S');
//                }else if(player_get_direction_facing(p) == 'E'){
//                    player_set_direction(p, 'W');
//                }else if(player_get_direction_facing(p) == 'S'){
//                    player_set_direction(p, 'N');
//                }else{
//                    player_set_direction(p, 'E');
//                }
//                key[KEY_DOWN] = true;
//                printf("Key down pressed\n");
//                redraw = true;
//                break;

            case ALLEGRO_KEY_LEFT:
                if(player_get_direction_facing(p) == 'N'){
                    player_set_direction(p, 'W');
                }else if(player_get_direction_facing(p) == 'E'){
                    player_set_direction(p, 'N');
                }else if(player_get_direction_facing(p) == 'S'){
                    player_set_direction(p, 'E');
                }else if(player_get_direction_facing(p) == 'W'){
                    player_set_direction(p, 'S');
                }
                key[KEY_LEFT] = true;
                printf("Key left pressed\n");
                redraw = true;
                break;

            case ALLEGRO_KEY_RIGHT:
                if(player_get_direction_facing(p) == 'N'){
                    player_set_direction(p, 'E');
                }else if(player_get_direction_facing(p) == 'E'){
                    player_set_direction(p, 'S');
                }else if(player_get_direction_facing(p) == 'S'){
                    player_set_direction(p, 'W');
                }else if(player_get_direction_facing(p) == 'W'){
                    player_set_direction(p, 'N');
                }
                key[KEY_RIGHT] = true;
                printf("Key right pressed\n");
                redraw = true;
                break;

            default:
                printf("Key was pressed - it wasn't an arrow key..\n");
                redraw = false;
                break;
        }
    }
}

void update_graphics(){
    if(key[KEY_RIGHT]){
        if(player_get_direction_facing(p) == 'N'){
            al_draw_bitmap(map[p->x][p->y].N,0,0,0);
        }else if(player_get_direction_facing(p) == 'E'){
            al_draw_bitmap(map[p->x][p->y].E,0,0,0);
        }else if(player_get_direction_facing(p) == 'S'){
            al_draw_bitmap(map[p->x][p->y].S,0,0,0);
        }else if(player_get_direction_facing(p) == 'W'){
            al_draw_bitmap(map[p->x][p->y].W,0,0,0);
        }
    }else if(key[KEY_LEFT]){
        if(player_get_direction_facing(p) == 'N'){
            al_draw_bitmap(map[p->x][p->y].N,0,0,0);
        }else if(player_get_direction_facing(p) == 'E'){
            al_draw_bitmap(map[p->x][p->y].E,0,0,0);
        }else if(player_get_direction_facing(p) == 'S'){
            al_draw_bitmap(map[p->x][p->y].S,0,0,0);
        }else if(player_get_direction_facing(p) == 'W'){
            al_draw_bitmap(map[p->x][p->y].W,0,0,0);
        }
    }else if(key[KEY_UP]){
        if(player_get_direction_facing(p) == 'N'){
            al_draw_bitmap(map[player_get_position_x(p)][player_get_position_y(p)].N,0,0,0);
        }else if(player_get_direction_facing(p) == 'E'){
            al_draw_bitmap(map[p->x][p->y].E,0,0,0);
        }else if(player_get_direction_facing(p) == 'S'){
            al_draw_bitmap(map[p->x][p->y].S,0,0,0);
        }else if(player_get_direction_facing(p) == 'W'){
            al_draw_bitmap(map[p->x][p->y].W,0,0,0);
        }
    }
//    else if(key[KEY_DOWN]){
//        if(player_get_direction_facing(p) == 'N'){
//            al_draw_bitmap(map[p->x][p->y].N,0,0,0);
//        }else if(player_get_direction_facing(p) == 'E'){
//            al_draw_bitmap(map[p->x][p->y].E,0,0,0);
//        }else if(player_get_direction_facing(p) == 'S'){
//            al_draw_bitmap(map[p->x][p->y].S,0,0,0);
//        }else if(player_get_direction_facing(p) == 'W'){
//            al_draw_bitmap(map[p->x][p->y].W,0,0,0);
//        }
//    }
    al_flip_display();
}

void abort_game(const char* message)
{
    printf("%s \n", message);
    exit(1);
}

void init(void)
{

    if (!al_init())
        abort_game("Failed to initialize allegro");

    if (!al_install_keyboard())
        abort_game("Failed to install keyboard");

    timer = al_create_timer(1.0 / 60);
    if (!timer)
        abort_game("Failed to create timer");

    if(!al_init_image_addon()) {
        abort_game("Failed to initialize al_init_image_addon!");
    }

    al_set_new_display_flags(ALLEGRO_WINDOWED);
    display = al_create_display(480, 480);
    if (!display)
        abort_game("Failed to create display");


    ALLEGRO_BITMAP *image = al_load_bitmap("/Users/anaruhudson/CLionProjects/EtC-1/assets/Example_corridor.JPG");

    al_draw_bitmap(image ,0, 0, 0);
    al_flip_display();


    event_queue = al_create_event_queue();
    if (!event_queue)
        abort_game("Failed to create event queue");

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));

    done = false;
}

void shutdown(void)
{
    if (timer)
        al_destroy_timer(timer);

    if (display)
        al_destroy_display(display);

    if (event_queue)
        al_destroy_event_queue(event_queue);

    if(p)
        player_destruct(p);
}


void game_loop(void)
{
    redraw = true;
    al_start_timer(timer);

    while (!done) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);


        if (event.type == ALLEGRO_EVENT_TIMER) {
            redraw = false;
            //update_logic();
        }else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            printf("Exit Pressed\n");
            done = true;
        }
        // Get user keyboard input
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            // Game ends if escape key pressed
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                printf("Escaped Pressed\n");
                done = true;
            }
            get_user_input(event);
        }

        // Update graphics based on the input received from above
        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            update_graphics();
        }
    }
}


int main(int argc, char* argv[])
{
    init();
    init_map();
    init_player();
    game_loop();
    shutdown();
}
#include <allegro5/allegro5.h>
#include <stdio.h>
#include <stdlib.h>
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include "Player.h"

enum MYKEYS
{
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SPACE
};

struct player
{
    int x;
    int y;
    char direction_facing;
};

typedef struct
{
    ALLEGRO_BITMAP *N;
    ALLEGRO_BITMAP *E;
    ALLEGRO_BITMAP *S;
    ALLEGRO_BITMAP *W;
    bool passable;
} MapTile;

MapTile map[5][1];

bool done;
bool redraw;
bool key[5] = { false, false, false, false, false };
bool first = true;

ALLEGRO_EVENT_QUEUE* event_queue;
ALLEGRO_TIMER* timer;
ALLEGRO_DISPLAY* display;


void abort_game(const char* message)
{
    printf("%s \n", message);
    exit(1);
}

player* player_constructor (void)
{
    struct player* instance = malloc (sizeof(player));
    if(instance != NULL)
    {
        instance->x = 0;
        instance->y = 0;
        instance->direction_facing = 'N';
    }
    else
    {
        abort_game("Failed to load Player");
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

void player_set_direction(player* this, char d)
{
    this->direction_facing = d;
}

char player_get_direction_facing(const player* this)
{
    return this->direction_facing;
}


void player_destruct (player* this)
{
    free(this);
}

player* p;

/**
 * Initialises the map. It's hard coded, but will eventually be read in from a file?
 */
void init_map(void)
{

    map[0][0].N = al_load_bitmap("ETC_5X1_CORRIDOR_GFX/1_N.bmp");
    map[0][0].E = al_load_bitmap("ETC_5X1_CORRIDOR_GFX/1_E.bmp");
    map[0][0].S = al_load_bitmap("ETC_5X1_CORRIDOR_GFX/1_S.bmp");
    map[0][0].W = al_load_bitmap("ETC_5X1_CORRIDOR_GFX/1_W.bmp");
    map[0][0].passable = true;

    map[1][0].N = al_load_bitmap("ETC_5X1_CORRIDOR_GFX/2_N.bmp");
    map[1][0].E = al_load_bitmap("ETC_5X1_CORRIDOR_GFX/2_E.bmp");
    map[1][0].S = al_load_bitmap("ETC_5X1_CORRIDOR_GFX/2_S.bmp");
    map[1][0].W = al_load_bitmap("ETC_5X1_CORRIDOR_GFX/2_W.bmp");
    map[1][0].passable = true;

    map[2][0].N = al_load_bitmap("ETC_5X1_CORRIDOR_GFX/3_N.bmp");
    map[2][0].E = al_load_bitmap("ETC_5X1_CORRIDOR_GFX/3_E.bmp");
    map[2][0].S = al_load_bitmap("ETC_5X1_CORRIDOR_GFX/3_S.bmp");
    map[2][0].W = al_load_bitmap("ETC_5X1_CORRIDOR_GFX/3_W.bmp");
    map[2][0].passable = true;

    map[3][0].N = al_load_bitmap("ETC_5X1_CORRIDOR_GFX/4_N.bmp");
    map[3][0].E = al_load_bitmap("ETC_5X1_CORRIDOR_GFX/4_E.bmp");
    map[3][0].S = al_load_bitmap("ETC_5X1_CORRIDOR_GFX/4_S.bmp");
    map[3][0].W = al_load_bitmap("ETC_5X1_CORRIDOR_GFX/4_W.bmp");
    map[3][0].passable = true;

    map[4][0].N = al_load_bitmap("ETC_5X1_CORRIDOR_GFX/5_N.bmp");
    map[4][0].E = al_load_bitmap("ETC_5X1_CORRIDOR_GFX/5_E.bmp");
    map[4][0].S = al_load_bitmap("ETC_5X1_CORRIDOR_GFX/5_S.bmp");
    map[4][0].W = al_load_bitmap("ETC_5X1_CORRIDOR_GFX/5_W.bmp");
    map[4][0].passable = true;
}

/**
 * Creates the Player.
 */
void init_player(void)
{
    p = player_constructor();
}

/**
 * Takes the key press event, and checks if it was either
 * up, left or right key pressed.
 * If up arrow key - update the player location either +1 if facing North, or -1 if facing South.
 * If left or right arrow key, update direction player facing.
 * @param event ALLEGRO_EVENT ---
 */
void get_user_input(ALLEGRO_EVENT event)
{
    if(event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        switch(event.keyboard.keycode)
        {
            case ALLEGRO_KEY_UP:
                if (first)
                {
                    break;
                }
                else
                {
                    if (player_get_direction_facing(p) == 'N' && map[player_get_position_x(p) + 1][player_get_position_y(p)].passable)
                    {
                        player_set_position(p, p->x + 1, p->y);
                        key[KEY_UP] = true;
                        printf("Key up pressed\n");
                        redraw = true;
                    }
                    else if (player_get_direction_facing(p) == 'S' && map[player_get_position_x(p) - 1][player_get_position_y(p)].passable)
                    {
                        player_set_position(p, p->x - 1, p->y);
                        key[KEY_UP] = true;
                        printf("Key up pressed\n");
                        redraw = true;
                    }
                    else
                    {
                        printf("Key up pressed, but can't advance..\n");
                    }
                    break;
                }

            case ALLEGRO_KEY_LEFT:
                if (first)
                {
                    break;
                }
                else
                {
                    if (player_get_direction_facing(p) == 'N')
                    {
                        player_set_direction(p, 'W');
                    }
                    else if (player_get_direction_facing(p) == 'E')
                    {
                        player_set_direction(p, 'N');
                    }
                    else if (player_get_direction_facing(p) == 'S')
                    {
                        player_set_direction(p, 'E');
                    }
                    else if (player_get_direction_facing(p) == 'W')
                    {
                        player_set_direction(p, 'S');
                    }
                    key[KEY_LEFT] = true;
                    printf("Key left pressed\n");
                    redraw = true;
                    break;
                }

            case ALLEGRO_KEY_RIGHT:
                if (first)
                {
                    break;
                }
                else
                {
                    if (player_get_direction_facing(p) == 'N')
                    {
                        player_set_direction(p, 'E');
                    }
                    else if (player_get_direction_facing(p) == 'E')
                    {
                        player_set_direction(p, 'S');
                    }
                    else if (player_get_direction_facing(p) == 'S')
                    {
                        player_set_direction(p, 'W');
                    }
                    else if (player_get_direction_facing(p) == 'W')
                    {
                        player_set_direction(p, 'N');
                    }
                    key[KEY_RIGHT] = true;
                    printf("Key right pressed\n");
                    redraw = true;
                    break;
                }

            case ALLEGRO_KEY_ESCAPE:
                break;

            case ALLEGRO_KEY_SPACE:
                if(first)
                {
                    key[KEY_SPACE] = true;
                    redraw = true;
                    break;
                }
                else
                {
                    break;
                }

            default:
                if (first)
                {
                    break;
                }
                else
                {
                    printf("Key was pressed - it wasn't an arrow key..\n");
                    redraw = false;
                    break;
                }
        }
    }
}

/**
 * Check if one of the keys has been pressed,
 * then update the graphics based on the new direction the player is facing.
 */
void update_graphics()
{
    if(first && key[KEY_SPACE])
    {
        first = false;
        if(player_get_direction_facing(p) == 'N') {
            al_draw_scaled_bitmap(map[player_get_position_x(p)][player_get_position_y(p)].N, 0, 0, 120, 120, 0, 0, 480,
                                  480, 0);
        }
        else if(player_get_direction_facing(p) == 'E')
        {
            al_draw_scaled_bitmap(map[player_get_position_x(p)][player_get_position_y(p)].E, 0, 0, 120, 120, 0, 0, 480, 480, 0);
        }
        else if(player_get_direction_facing(p) == 'S')
        {
            al_draw_scaled_bitmap(map[player_get_position_x(p)][player_get_position_y(p)].S, 0, 0, 120, 120, 0, 0, 480, 480, 0);
        }
        else if(player_get_direction_facing(p) == 'W')
        {
            al_draw_scaled_bitmap(map[player_get_position_x(p)][player_get_position_y(p)].W, 0, 0, 120, 120, 0, 0, 480, 480, 0);
        }
    }
    else if(key[KEY_RIGHT] || key[KEY_LEFT] || key[KEY_UP])
    {
        if(player_get_direction_facing(p) == 'N')
        {
            al_draw_scaled_bitmap(map[player_get_position_x(p)][player_get_position_y(p)].N, 0, 0, 120, 120, 0, 0, 480, 480, 0);
        }
        else if(player_get_direction_facing(p) == 'E')
        {
            al_draw_scaled_bitmap(map[player_get_position_x(p)][player_get_position_y(p)].E, 0, 0, 120, 120, 0, 0, 480, 480, 0);
        }
        else if(player_get_direction_facing(p) == 'S')
        {
            al_draw_scaled_bitmap(map[player_get_position_x(p)][player_get_position_y(p)].S, 0, 0, 120, 120, 0, 0, 480, 480, 0);
        }
        else if(player_get_direction_facing(p) == 'W')
        {
            al_draw_scaled_bitmap(map[player_get_position_x(p)][player_get_position_y(p)].W, 0, 0, 120, 120, 0, 0, 480, 480, 0);
        }
    }
    al_flip_display();
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

    if(!al_init_image_addon())
    {
        abort_game("Failed to initialize al_init_image_addon!");
    }

    al_set_new_display_flags(ALLEGRO_WINDOWED);
    display = al_create_display(480, 480);
    if (!display)
        abort_game("Failed to create display");

    ALLEGRO_BITMAP *image = al_load_bitmap("ETC_5X1_CORRIDOR_GFX/EtC_Alpha_Titlescreen.bmp");
    if(!image)
        abort_game("Failed to load starting image");
    al_draw_scaled_bitmap(image, 0, 0, 120, 120, 0, 0, 480, 480, 0);
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

    while (!done)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);


        if (event.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = false;
            //update_logic();
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            printf("Exit Pressed\n");
            done = true;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            // Game ends if escape key pressed
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                printf("Escaped Pressed\n");
                done = true;
            }
            get_user_input(event);
        }

        // Update graphics based on the input received from above
        if (redraw && al_is_event_queue_empty(event_queue))
        {
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
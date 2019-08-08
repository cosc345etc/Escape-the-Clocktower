#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "Player.h"

#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))


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
    SDL_Surface *N;
    SDL_Surface *E;
    SDL_Surface *S;
    SDL_Surface *W;
    bool passable_from_N;
    bool passable_from_E;
    bool passable_from_S;
    bool passable_from_W;

} MapTile;

static const int width = 240;
static const int height = 240;

bool done = false;
bool redraw;
bool key[5] = { false, false, false, false, false };
bool first = true;
MapTile map[5][2];
int columns;
int rows;

SDL_Window * window;
SDL_Renderer * renderer;
SDL_Surface * loadImage;
SDL_Texture * texture;

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

void init_map(void)
{

    map[0][0].N = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/1_N.bmp");
//    map[0][0].E = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/1_E.bmp");
    map[0][0].E = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/5_N.bmp");
    map[0][0].S = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/1_S.bmp");
    map[0][0].W = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/1_W.bmp");
    map[0][0].passable_from_N = true;
    map[0][0].passable_from_E = false;
    map[0][0].passable_from_S = false;
    map[0][0].passable_from_W = true;

    map[0][1].N = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/1_S.bmp");
    map[0][1].E = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/1_S.bmp");
    map[0][1].S = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/1_S.bmp");
    map[0][1].W = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/5_N.bmp");
    map[0][1].passable_from_N = true;
    map[0][1].passable_from_E = true;
    map[0][1].passable_from_S = true;
    map[0][1].passable_from_W = false;

    map[1][0].N = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/2_N.bmp");
    map[1][0].E = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/2_E.bmp");
    map[1][0].S = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/2_S.bmp");
    map[1][0].W = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/2_W.bmp");
    map[1][0].passable_from_N = true;
    map[1][0].passable_from_E = false;
    map[1][0].passable_from_S = true;
    map[1][0].passable_from_W = false;

    map[1][1].N = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/2_N.bmp");
    map[1][1].E = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/2_E.bmp");
    map[1][1].S = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/2_S.bmp");
    map[1][1].W = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/2_W.bmp");
    map[1][1].passable_from_N = true;
    map[1][1].passable_from_E = false;
    map[1][1].passable_from_S = true;
    map[1][1].passable_from_W = false;

    map[2][0].N = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/3_N.bmp");
    map[2][0].E = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/3_E.bmp");
    map[2][0].S = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/3_S.bmp");
    map[2][0].W = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/3_W.bmp");
    map[2][0].passable_from_N = true;
    map[2][0].passable_from_E = false;
    map[2][0].passable_from_S = true;
    map[2][0].passable_from_W = false;

    map[2][1].N = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/3_N.bmp");
    map[2][1].E = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/3_E.bmp");
    map[2][1].S = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/3_S.bmp");
    map[2][1].W = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/3_W.bmp");
    map[2][1].passable_from_N = true;
    map[2][1].passable_from_E = false;
    map[2][1].passable_from_S = true;
    map[2][1].passable_from_W = false;

    map[3][0].N = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/4_N.bmp");
    map[3][0].E = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/4_E.bmp");
    map[3][0].S = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/4_S.bmp");
    map[3][0].W = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/4_W.bmp");
    map[3][0].passable_from_N = true;
    map[3][0].passable_from_E = false;
    map[3][0].passable_from_S = true;
    map[3][0].passable_from_W = false;

    map[3][1].N = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/4_N.bmp");
    map[3][1].E = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/4_E.bmp");
    map[3][1].S = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/4_S.bmp");
    map[3][1].W = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/4_W.bmp");
    map[3][1].passable_from_N = true;
    map[3][1].passable_from_E = false;
    map[3][1].passable_from_S = true;
    map[3][1].passable_from_W = false;

    map[4][0].N = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/5_N.bmp");
    map[4][0].E = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/5_E.bmp");
    map[4][0].S = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/5_S.bmp");
    map[4][0].W = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/5_W.bmp");
    map[4][0].passable_from_N = false;
    map[4][0].passable_from_E = false;
    map[4][0].passable_from_S = true;
    map[4][0].passable_from_W = false;

    map[4][1].N = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/5_N.bmp");
    map[4][1].E = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/5_E.bmp");
    map[4][1].S = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/5_S.bmp");
    map[4][1].W = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/5_W.bmp");
    map[4][1].passable_from_N = false;
    map[4][1].passable_from_E = false;
    map[4][1].passable_from_S = true;
    map[4][1].passable_from_W = false;

    printf("Number of rows: %d\n", LEN(map));
    columns = LEN(map);
    printf("Number of columns: %d\n", LEN(map[0]));
    rows = LEN(map[0]);
}

void print_pos(player* p){
    printf("%d, %d\n", p->x, p->y);
    printf("%c\n", p->direction_facing);
}

void init_player(void)
{
    p = player_constructor();
    printf("Just to double check\n");
    printf("columns: %d\n", columns);
    printf("rows: %d\n", rows);
}

void get_user_input(SDL_Event event)
{
    if(event.type == SDL_KEYDOWN)
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_UP:
                if (first)
                {
                    break;
                }
                else
                {
                    if (player_get_direction_facing(p) == 'N' && map[player_get_position_x(p) + 1][player_get_position_y(p)].passable_from_S && (player_get_position_x(p) + 1) < columns)
                    {
                        print_pos(p);
                        player_set_position(p, p->x + 1, p->y);
                        print_pos(p);
                        printf("-------\n");
                        key[KEY_UP] = true;
//                        printf("Key up pressed\n");
                        redraw = true;
                    }
                    else if (player_get_direction_facing(p) == 'S' && map[player_get_position_x(p) - 1][player_get_position_y(p)].passable_from_N && (player_get_position_x(p) - 1 ) >= 0)
                    {
                        print_pos(p);
                        player_set_position(p, p->x - 1, p->y);
                        print_pos(p);
                        printf("-------\n");
                        key[KEY_UP] = true;
//                        printf("Key up pressed\n");
                        redraw = true;
                    }
                    else if(player_get_direction_facing(p) == 'E' && map[player_get_position_x(p)][player_get_position_y(p) + 1].passable_from_E && (player_get_position_y(p) + 1) < rows)
                    {
                        print_pos(p);
                        player_set_position(p, p->x, p->y+1);
                        print_pos(p);
                        printf("-------\n");
                        key[KEY_UP] = true;
//                        printf("Key up pressed\n");
                        redraw = true;
                    }
                    else if(player_get_direction_facing(p) == 'W' && map[player_get_position_x(p)][player_get_position_y(p) - 1].passable_from_W && (player_get_position_y(p) - 1) >= 0){
                        print_pos(p);
                        player_set_position(p, p->x, p->y-1);
                        print_pos(p);
                        printf("-------\n");
                        key[KEY_UP] = true;
//                        printf("Key up pressed\n");
                        redraw = true;
                    }
                    else
                    {
                        printf("Key up pressed, but can't advance..\n");
                    }
                    break;
                }

            case SDLK_LEFT:
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
//                    printf("Key left pressed\n");
                    redraw = true;
                    break;
                }

            case SDLK_RIGHT:
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
//                    printf("Key right pressed\n");
                    redraw = true;
                    break;
                }

            case SDLK_ESCAPE:
                break;

            case SDLK_SPACE:
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
//                    printf("Key was pressed - it wasn't an arrow key..\n");
                    redraw = false;
                    break;
                }
        }
    }
}

void update_graphics()
{
    if(first && key[KEY_SPACE])
    {
        first = false;
        if(player_get_direction_facing(p) == 'N') {
            texture = SDL_CreateTextureFromSurface(renderer, map[player_get_position_x(p)][player_get_position_y(p)].N);
        }
        else if(player_get_direction_facing(p) == 'E')
        {
            texture = SDL_CreateTextureFromSurface(renderer, map[player_get_position_x(p)][player_get_position_y(p)].E);
        }
        else if(player_get_direction_facing(p) == 'S')
        {
            texture = SDL_CreateTextureFromSurface(renderer, map[player_get_position_x(p)][player_get_position_y(p)].S);
        }
        else if(player_get_direction_facing(p) == 'W')
        {
            texture = SDL_CreateTextureFromSurface(renderer, map[player_get_position_x(p)][player_get_position_y(p)].W);
        }
    }
    else if(key[KEY_RIGHT] || key[KEY_LEFT] || key[KEY_UP])
    {
        if(player_get_direction_facing(p) == 'N')
        {
            texture = SDL_CreateTextureFromSurface(renderer, map[player_get_position_x(p)][player_get_position_y(p)].N);
        }
        else if(player_get_direction_facing(p) == 'E')
        {
            texture = SDL_CreateTextureFromSurface(renderer, map[player_get_position_x(p)][player_get_position_y(p)].E);
        }
        else if(player_get_direction_facing(p) == 'S')
        {
            texture = SDL_CreateTextureFromSurface(renderer, map[player_get_position_x(p)][player_get_position_y(p)].S);
        }
        else if(player_get_direction_facing(p) == 'W')
        {
            texture = SDL_CreateTextureFromSurface(renderer, map[player_get_position_x(p)][player_get_position_y(p)].W);
        }
    }
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void shutdown(void)
{

    if(p){
        player_destruct(p);
    }

    if(texture){
        SDL_DestroyTexture(texture);
    }

    if(loadImage){
        SDL_FreeSurface(loadImage);
    }

    if(renderer){
        SDL_DestroyRenderer(renderer);
    }

    if(window){
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
}

void init(void)
{

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Escape the Clocktower", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    if(!window){
        abort_game("Failed to create window");
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer){
        abort_game("Failed to create renderer");
    }
    loadImage = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/EtC_Alpha_Titlescreen.bmp");
    if(!loadImage){
        abort_game("Failed to load starting image");
    }

    texture = SDL_CreateTextureFromSurface(renderer, loadImage);
    if(!texture){
        abort_game("Failed to load texture");
    }

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void game_loop(void)
{
    redraw = true;


    while (!done)
    {
        SDL_Event event;
        SDL_WaitEvent(&event);


        if (event.type == SDL_QUIT)
        {
            printf("Exit Pressed\n");
            done = true;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            // Game ends if escape key pressed
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                printf("Escaped Pressed\n");
                done = true;
            }
            get_user_input(event);
        }

        if (redraw) // TODO: possibly figure out event queue?
        {
            redraw = false;
            update_graphics();
        }
    }
}



int main(int argc, char ** argv)
{
    init();
    init_map();
    init_player();
    game_loop();
    shutdown();
    exit(0);
}

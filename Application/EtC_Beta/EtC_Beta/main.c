#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#include <SDL2/SDL.h>

#pragma clang diagnostic pop
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
    SDL_Surface *N;
    SDL_Surface *E;
    SDL_Surface *S;
    SDL_Surface *W;
    bool passable_from_N;
    bool passable_from_E;
    bool passable_from_S;
    bool passable_from_W;
    SDL_Surface *interact_image;
    bool is_interactive;
    char dir_need_to_face;
} MapTile;

static const int width = 480;
static const int height = 480;

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

void read_line_to_map(char *s, int x, int y)
{
    char delim[] = " ";
    
    char *ptr = strtok(s, delim);
    int counter = 0;
    while(ptr != NULL)
    {
        if(counter == 0){
            map[x][y].N = SDL_LoadBMP(ptr);
        }else if(counter == 1){
            map[x][y].E = SDL_LoadBMP(ptr);
        }else if(counter == 2){
            map[x][y].S = SDL_LoadBMP(ptr);
        }else if(counter == 3){
            map[x][y].W = SDL_LoadBMP(ptr);
        }else if(counter == 4){
            bool b;
            if(strcmp(ptr,"true") == 0 )
                b = true;
            else
                b = false;
            map[x][y].passable_from_N = b;
        }else if(counter == 5){
            bool b;
            if(strcmp(ptr,"true") == 0 )
                b = true;
            else
                b = false;
            map[x][y].passable_from_E = b;
        }else if(counter == 6){
            bool b;
            if(strcmp(ptr,"true") == 0 )
                b = true;
            else
                b = false;
            map[x][y].passable_from_S = b;
        }else if(counter == 7){
            bool b;
            if(strcmp(ptr,"true") == 0 )
                b = true;
            else
                b = false;
            map[x][y].passable_from_W = b;
        }else if(counter == 8){
            bool b;
            if(strcmp(ptr,"true") == 0 )
                b = true;
            else
                b = false;
            map[x][y].is_interactive = b;
        }else if(counter == 9){
            if(strcmp(ptr,"NULL") == 0){
                map[x][y].interact_image = NULL;
            }else{
                map[x][y].interact_image = SDL_LoadBMP(ptr);
            }
        }else{
            if(strcmp(ptr, "(char)0\n") == 0){
                map[x][y].dir_need_to_face = (char)0;
            }else{
                map[x][y].dir_need_to_face = *ptr;
            }
        }
        ptr = strtok(NULL, delim);
        counter++;
    }
    
}

void load_map_file(char *fname)
{
    FILE* file = fopen(fname, "r");
    if(file == NULL)
    {
        abort_game("Failed to load map .txt file");
    }
    char line[256];
    int i, j;
    for(i = 0; i < columns; i++)
    {
        for(j = 0; j < rows; j++)
        {
            fgets(line, sizeof(line), file);
            read_line_to_map(line, i, j);
        }
    }
    fclose(file);
}

void init_map(void)
{
    columns = LEN(map);
    rows = LEN(map[0]);
    load_map_file("map-beta.txt");
}

void print_pos(player* p){
    printf("%d, %d\n", p->x, p->y);
    printf("%c\n", p->direction_facing);
}

void init_player(void)
{
    p = player_constructor();
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
                        redraw = true;
                    }
                    else if (player_get_direction_facing(p) == 'S' && map[player_get_position_x(p) - 1][player_get_position_y(p)].passable_from_N && (player_get_position_x(p) - 1 ) >= 0)
                    {
                        print_pos(p);
                        player_set_position(p, p->x - 1, p->y);
                        print_pos(p);
                        printf("-------\n");
                        key[KEY_UP] = true;
                        redraw = true;
                    }
                    else if(player_get_direction_facing(p) == 'E' && map[player_get_position_x(p)][player_get_position_y(p) + 1].passable_from_E && (player_get_position_y(p) + 1) < rows)
                    {
                        print_pos(p);
                        player_set_position(p, p->x, p->y+1);
                        print_pos(p);
                        printf("-------\n");
                        key[KEY_UP] = true;
                        redraw = true;
                    }
                    else if(player_get_direction_facing(p) == 'W' && map[player_get_position_x(p)][player_get_position_y(p) - 1].passable_from_W && (player_get_position_y(p) - 1) >= 0){
                        print_pos(p);
                        player_set_position(p, p->x, p->y-1);
                        print_pos(p);
                        printf("-------\n");
                        key[KEY_UP] = true;
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
                    redraw = true;
                    break;
                }
                
            case SDLK_ESCAPE:
                
                break;
                
            case SDLK_SPACE:
                printf("Space pressed!\n");
                key[KEY_SPACE] = true;
                redraw = true;
                break;
            default:
                if (first)
                {
                    break;
                }
                else
                {
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
        key[KEY_SPACE] = false;
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
    else if(key[KEY_SPACE])
    {
        key[KEY_SPACE] = false;
        if(map[player_get_position_x(p)][player_get_position_y(p)].is_interactive && player_get_direction_facing(p) == map[player_get_position_x(p)][player_get_position_y(p)].dir_need_to_face)
        {
            texture = SDL_CreateTextureFromSurface(renderer, map[player_get_position_x(p)][player_get_position_y(p)].interact_image);
            map[player_get_position_x(p)][player_get_position_y(p)].is_interactive = false;
            map[player_get_position_x(p)][player_get_position_y(p)].N = map[player_get_position_x(p)][player_get_position_y(p)].interact_image;
            
        }
    }
    else if(key[KEY_RIGHT] || key[KEY_LEFT] || key[KEY_UP])
    {
        key[KEY_RIGHT] = false;
        key[KEY_LEFT] = false;
        key[KEY_DOWN] = false;
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

void gameshutdown(void)
{
    
    if(p)
    {
        player_destruct(p);
    }
    
    if(texture)
    {
        SDL_DestroyTexture(texture);
    }
    
    if(loadImage)
    {
        SDL_FreeSurface(loadImage);
    }
    
    if(renderer)
    {
        SDL_DestroyRenderer(renderer);
    }
    
    if(window)
    {
        SDL_DestroyWindow(window);
    }
    
    SDL_Quit();
}

void init(void)
{
    SDL_Init(SDL_INIT_VIDEO);
    
    window = SDL_CreateWindow("Escape the Clocktower", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    if(!window)
    {
        abort_game("Failed to create window");
    }
    
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer)
    {
        abort_game("Failed to create renderer");
    }
    
    loadImage = SDL_LoadBMP("ETC_5X1_CORRIDOR_GFX/EtC_Alpha_Titlescreen.bmp");
    if(!loadImage)
    {
        abort_game("Failed to load starting image");
    }
    
    texture = SDL_CreateTextureFromSurface(renderer, loadImage);
    if(!texture)
    {
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
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                printf("Escaped Pressed\n");
                done = true;
            }
            get_user_input(event);
        }
        if (redraw)
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
    gameshutdown();
    exit(0);
}


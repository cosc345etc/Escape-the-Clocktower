<<<<<<< HEAD
#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "Player.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <SDL2/SDL.h>
#pragma clang diagnostic pop

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

typedef struct varray_t
{
    void **memory;
    size_t allocated;
    size_t used;
    int index;
    int num_used;
} varray;

static const int width = 480;
static const int height = 480;

bool done = false;
bool redraw;
bool key[5] = { false, false, false, false, false };

int num_intro_screens;
int array_pos = 0;
char *map_filename;
int columns;
int rows;

MapTile map[9][8];

SDL_Rect view_rect = {0, 0, 320, 320};
SDL_Rect text_rect = {0, 320, 480, 160};
SDL_Window * window;

SDL_Renderer * renderer;
SDL_Texture * texture;

SDL_Surface * border_surface;
SDL_Texture * border_texture;

SDL_Surface * text_surface;
SDL_Texture * text_texture;

void abort_game(const char* message)
{
    printf("%s \n", message);
    exit(1);
}

varray *garray;

void varray_init(varray **array)
{
    *array = (varray*)malloc(sizeof(varray));
    (*array)->memory = NULL;
    (*array)->allocated = 0;
    (*array)->used = 0;
    (*array)->index = -1;
    (*array)->num_used = 0;
}

void varray_push(varray *array, void *data)
{
    size_t toallocate;
    size_t size = sizeof(void*);
    if ((array->allocated - array->used) < size) {
        toallocate = array->allocated == 0 ? size : (array->allocated * 2);
        array->memory = realloc(array->memory, toallocate);
        array->allocated = toallocate;
    }
    
    array->memory[++array->index] = data;
    array->used = array->used + size;
    array->num_used += 1;
}

int varray_length(varray *array)
{
    return array->index + 1;
}

void varray_insert(varray *array, int index, void *data)
{
    if (index < 0 || index > array->index)
        return;
    
    array->memory[index] = data;
}

void varray_free(varray *array)
{
    for(int i  = 0; i < array->num_used; i++){
        if(array->memory[i] != NULL){
            SDL_FreeSurface(array->memory[i]);
        }
    }
    free(array->memory);
    free(array);
}

void* varray_get(varray *array, int index)
{
    if (index < 0 || index > array->index)
        return NULL;
    
    return array->memory[index];
}

void check_image_loaded(SDL_Surface *image){
    if(!image)
    {
        abort_game("Failed to load image");
    }
}

void check_texture_loaded(SDL_Texture * texture){
    if(!texture)
    {
        abort_game("Failed to load texture");
    }
}

void test_free_map_array(){
    for(int i = 0; i < columns; i++)
    {
        for(int j = 0; j < rows; j++)
        {
            if(map[i][j].N != NULL)
            {
                SDL_FreeSurface(map[i][j].N);
                map[i][j].N = NULL;
            }
            if(map[i][j].E != NULL)
            {
                SDL_FreeSurface(map[i][j].E);
                map[i][j].E = NULL;
            }
            if(map[i][j].S != NULL)
            {
                SDL_FreeSurface(map[i][j].S);
                map[i][j].S = NULL;
            }
            if(map[i][j].W != NULL)
            {
                SDL_FreeSurface(map[i][j].W);
                map[i][j].W = NULL;
            }
            if(map[i][j].interact_image != NULL)
            {
                SDL_FreeSurface(map[i][j].interact_image);
                map[i][j].interact_image = NULL;
            }
        }
    }
}

player* player_constructor (int x, int y, char dir)
{
    struct player* instance = malloc (sizeof(player));
    if(instance != NULL)
    {
        instance->x = x;
        instance->y = y;
        instance->direction_facing = dir;
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

void gameshutdown(void)
{
    
    if(p)
    {
        player_destruct(p);
    }
    
    if(map_filename)
    {
        free(map_filename);
    }
    
    if(border_texture)
    {
        SDL_DestroyTexture(border_texture);
    }
    
    if(text_texture)
    {
        SDL_DestroyTexture(text_texture);
    }
    
    if(texture)
    {
        SDL_DestroyTexture(texture);
    }
    
    if(border_surface)
    {
        SDL_FreeSurface(border_surface);
    }
    
    if(renderer)
    {
        SDL_DestroyRenderer(renderer);
    }
    
    if(window)
    {
        SDL_DestroyWindow(window);
    }
    
//    free_image_array(&a);
    varray_free(garray);
    
    SDL_Quit();
}


void get_user_input(SDL_Event event)
{
    if(event.type == SDL_KEYDOWN)
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_UP:
                if(num_intro_screens > 0){
                    break;
                }
                else if (player_get_direction_facing(p) == 'N' && map[player_get_position_x(p) + 1][player_get_position_y(p)].passable_from_S && (player_get_position_x(p) + 1) < columns)
                {
                    player_set_position(p, p->x + 1, p->y);
                    key[KEY_UP] = true;
                    redraw = true;
                }
                else if (player_get_direction_facing(p) == 'S' && map[player_get_position_x(p) - 1][player_get_position_y(p)].passable_from_N && (player_get_position_x(p) - 1 ) >= 0)
                {
                    player_set_position(p, p->x - 1, p->y);
                    key[KEY_UP] = true;
                    redraw = true;
                }
                else if(player_get_direction_facing(p) == 'E' && map[player_get_position_x(p)][player_get_position_y(p) + 1].passable_from_W && (player_get_position_y(p) + 1) < rows)
                {
                    player_set_position(p, p->x, p->y+1);
                    key[KEY_UP] = true;
                    redraw = true;
                }
                else if(player_get_direction_facing(p) == 'W' && map[player_get_position_x(p)][player_get_position_y(p) - 1].passable_from_E &&(player_get_position_y(p) - 1) >= 0)
                {
                    player_set_position(p, p->x, p->y-1);
                    key[KEY_UP] = true;
                    redraw = true;
                }
                else
                {
                    printf("Key up pressed, but can't advance..\n");
                }
                break;
                
            case SDLK_LEFT:
                if(num_intro_screens > 0){
                    break;
                }
                else if (player_get_direction_facing(p) == 'N')
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
                
            case SDLK_RIGHT:
                if(num_intro_screens > 0){
                    break;
                }
                else if (player_get_direction_facing(p) == 'N')
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
                
            case SDLK_ESCAPE:
                break;
                
            case SDLK_SPACE:
                if(player_get_position_x(p) == 4 && player_get_position_y(p) == 2)
                {
                    player_set_position(p, 2, 3);
                    player_set_direction(p, 'N');
                }
                key[KEY_SPACE] = true;
                redraw = true;
                break;
            default:
                redraw = false;
                break;
        }
    }
}

void graphics_show_direction_facing(){
    if(player_get_direction_facing(p) == 'N')
    {
        if(texture)
        {
            SDL_DestroyTexture(texture);
        }
        texture = SDL_CreateTextureFromSurface(renderer, map[player_get_position_x(p)][player_get_position_y(p)].N);
    }
    else if(player_get_direction_facing(p) == 'E')
    {
        if(texture)
        {
            SDL_DestroyTexture(texture);
        }
        texture = SDL_CreateTextureFromSurface(renderer, map[player_get_position_x(p)][player_get_position_y(p)].E);
    }
    else if(player_get_direction_facing(p) == 'S')
    {
        if(texture)
        {
            SDL_DestroyTexture(texture);
        }
        texture = SDL_CreateTextureFromSurface(renderer, map[player_get_position_x(p)][player_get_position_y(p)].S);
    }
    else if(player_get_direction_facing(p) == 'W')
    {
        if(texture)
        {
            SDL_DestroyTexture(texture);
        }
        texture = SDL_CreateTextureFromSurface(renderer, map[player_get_position_x(p)][player_get_position_y(p)].W);
    }
    check_texture_loaded(texture);
}

void graphics_render_multiple_texture()
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, &view_rect);
    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
    SDL_RenderCopy(renderer, border_texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void update_graphics()
{
    if(num_intro_screens > 1 && key[KEY_SPACE]){
        key[KEY_SPACE] = false;
        
        if(texture)
        {
            SDL_DestroyTexture(texture);
        }
        texture = SDL_CreateTextureFromSurface(renderer, varray_get(garray, array_pos));//&a.image[array_pos]
        check_texture_loaded(texture);
        
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        
        num_intro_screens--;
        array_pos++;
        return;
    }
    else if(num_intro_screens > 0 && key[KEY_SPACE])
    {
        key[KEY_SPACE] = false;
        
        graphics_show_direction_facing();
        
        graphics_render_multiple_texture();
        
        num_intro_screens--;
        
        return;
    }
    else if(key[KEY_SPACE])
    {
        key[KEY_SPACE] = false;
        if(map[player_get_position_x(p)][player_get_position_y(p)].is_interactive && player_get_direction_facing(p) == map[player_get_position_x(p)][player_get_position_y(p)].dir_need_to_face)
        {
            if(texture)
            {
                SDL_DestroyTexture(texture);
            }
            texture = SDL_CreateTextureFromSurface(renderer, map[player_get_position_x(p)][player_get_position_y(p)].interact_image);
            map[player_get_position_x(p)][player_get_position_y(p)].is_interactive = false;
            switch(player_get_direction_facing(p)){
                case 'N':
                    map[player_get_position_x(p)][player_get_position_y(p)].N = map[player_get_position_x(p)][player_get_position_y(p)].interact_image;
                    break;
                case 'E':
                    map[player_get_position_x(p)][player_get_position_y(p)].E = map[player_get_position_x(p)][player_get_position_y(p)].interact_image;
                    break;
                case 'S':
                    map[player_get_position_x(p)][player_get_position_y(p)].S = map[player_get_position_x(p)][player_get_position_y(p)].interact_image;
                    break;
                case 'W':
                    map[player_get_position_x(p)][player_get_position_y(p)].W = map[player_get_position_x(p)][player_get_position_y(p)].interact_image;
                    break;
                default:break;
            }
            graphics_render_multiple_texture();
            return;
        }
        else
        {
            graphics_show_direction_facing();
            
            graphics_render_multiple_texture();
            return;
        }
    }
    else if(key[KEY_RIGHT] || key[KEY_LEFT] || key[KEY_UP])
    {
        if(num_intro_screens > 0){
            key[KEY_RIGHT] = key[KEY_LEFT] = key[KEY_DOWN] = false;
            return;
        }
        key[KEY_RIGHT] = key[KEY_LEFT] = key[KEY_DOWN] = false;
        graphics_show_direction_facing();
        
        graphics_render_multiple_texture();
        return;
    }
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

void init_player(int x, int y, char dir)

{
    p = player_constructor(x, y, dir);
}

bool check_true_or_false(char* ptr)
{
    if(strcmp(ptr,"true") == 0 )
        return true;
    else
        return false;
}

void check_image_null_or_load(SDL_Surface **image, char *ptr){
    if(strcmp(ptr,"NULL") == 0 )
    {
        image = NULL;
    }
    else
    {
        *image = SDL_LoadBMP(ptr);
        check_image_loaded(*image);
    }
}

void read_line_to_map(char *s, int x, int y)
{
    char delim[] = " ";
    
    char *ptr = strtok(s, delim);
    int counter = 0;
    while(ptr != NULL)
    {
        switch(counter){
            case 0:
                check_image_null_or_load(&map[x][y].N, ptr);
                break;
            case 1:
                check_image_null_or_load(&map[x][y].E, ptr);
                break;
            case 2:
                check_image_null_or_load(&map[x][y].S, ptr);
                break;
            case 3:
                check_image_null_or_load(&map[x][y].W, ptr);
                break;
            case 4:
                map[x][y].passable_from_N = check_true_or_false(ptr);
                break;
            case 5:
                map[x][y].passable_from_E = check_true_or_false(ptr);
                break;
            case 6:
                map[x][y].passable_from_S = check_true_or_false(ptr);
                break;
            case 7:
                map[x][y].passable_from_W = check_true_or_false(ptr);
                break;
            case 8:
                map[x][y].is_interactive = check_true_or_false(ptr);
                break;
            case 9:
                check_image_null_or_load(&map[x][y].interact_image, ptr);
                break;
            case 10:
                if(strcmp(ptr, "(char)0") == 0){
                    map[x][y].dir_need_to_face = (char)0;
                }else{
                    map[x][y].dir_need_to_face = *ptr;
                }
                break;
            default: break;
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
        abort_game("Failed to load map.txt file");
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
    load_map_file(map_filename);
}

void load_starting_images_from_line(char *line)
{
    if (line && !line[0]) {
        return;
    }
    char delim[] = " ";
    char *ptr = strtok(line, delim);
    int line_counter = 0;
    while(ptr != NULL)
    {
        SDL_Surface *image = SDL_LoadBMP(ptr);
        varray_push(garray, image);
        line_counter++;
        ptr = strtok(NULL, delim);
    }
    num_intro_screens = line_counter;
}

void check_min_num_lines_file(FILE* file)
{
    int line_count = 0;
    char line[256];
    while(fgets(line, sizeof(line), file))
    {
        line_count++;
    }
    
    if(line_count != 5)
    {
        abort_game("init.txt does not contain the correct number of lines");
    }
}

void check_char_is_num_in_range(char *pos)
{
    if(*pos == '0')
    {
        return;
    }
    else if (atoi(pos) == 0){
        abort_game("Invalid starting co-ordinate");
    }
}

void load_starting_game_vars(int *x, int *y, char *dir)
{
    FILE* file = fopen("init.txt", "r");
    if(file == NULL)
    {
        abort_game("Failed to load init.txt file");
    }
    
    varray_init(&garray);
    check_min_num_lines_file(file);
    rewind(file);
    
    char line[256];
    int line_number = 1;
    while(fgets(line, sizeof(line), file))
    {
        char* pos = strrchr(line, '\n');
        if (pos) *pos = 0;
        switch(line_number)
        {
            case 1:
                map_filename = (char *)calloc(1, strlen(line) + 1);
                if(map_filename == NULL)
                {
                    abort_game("Failed to allocate memory to map filename");
                }
                strcpy(map_filename, line);
                break;
            case 2:
                load_starting_images_from_line(line);
                break;
            case 3:
                check_char_is_num_in_range(&line[0]);
                *x = atoi(line);
                break;
            case 4:
                check_char_is_num_in_range(&line[0]);
                *y = atoi(line);
                break;
            case 5:
                *dir = line[0];
                break;
            default: break;
        }
        line_number++;
    }
    fclose(file);
}

void init(void)
{
    int x, y;
    char dir;
    load_starting_game_vars(&x, &y, &dir);
    init_map();
    init_player(x, y, dir);
    
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
    
    border_surface = SDL_LoadBMP("FULL_FRAME_BORDER.bmp");
    if(!border_surface){
        abort_game("Failed to load FULL_FRAME_BORDER.bmp");
    }
    
    text_surface = SDL_LoadBMP("TEXT_BOX_TEMPLATE.bmp");
    if(!text_surface){
        abort_game("Failed to load TEXT_BOX_TEMPLATE.bmp");
    }
    
    border_texture = SDL_CreateTextureFromSurface(renderer, border_surface);
    if(!border_texture){
        abort_game("Failed to load border_texture");
    }
    
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if(!text_texture){
        abort_game("Failed to load text_texture");
    }
    
    if(varray_length(garray) == 0)
    {
        graphics_show_direction_facing();
        graphics_render_multiple_texture();
    }
    else
    {
        texture = SDL_CreateTextureFromSurface(renderer, varray_get(garray, array_pos));
        if(!texture)
        {
            abort_game("Failed to load texture");
        }
        array_pos++;
        
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
}

int main(int argc, char ** argv)
{
    init();
    game_loop();
    gameshutdown();
    exit(0);
}
||||||| merged common ancestors
=======
#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "Player.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <SDL2/SDL.h>
#pragma clang diagnostic pop
enum MYKEYS
{
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SPACE
};
struct player
{
    int x;
    int y;
    int uniform;
    int pineapple;
    int apple;
    int endgame;
    char direction_facing;
};
typedef struct
{
    SDL_Surface *N;
    SDL_Surface *E;
    SDL_Surface *S;
    SDL_Surface *W;
    SDL_Surface *interact_image;
    bool passable_from_N;
    bool passable_from_E;
    bool passable_from_S;
    bool passable_from_W;
    bool is_interactive;
    char *map_item, *speaker;
    char dir_need_to_face;
} MapTile;
typedef struct
{
    void **memory;
    size_t allocated;
    size_t used;
    int index, num_used;
} IntroImageArray;
int width = 480;
int height = 480;
int num_intro_screens;
int rows;
int columns;
int array_pos = 0;
bool redraw, done = false;
bool key[5] = { false, false, false, false, false };
char *map_filename;
MapTile map[9][8];
IntroImageArray *image_array;
player* p;
SDL_Rect view_rect = {0, 0, 320, 320};
SDL_Rect text_rect = {0, 320, 480, 160};
SDL_Rect inv_rect = {320, 0, 160, 320};
SDL_Window * window;
SDL_Renderer * renderer;
SDL_Texture * texture;
SDL_Texture * border_texture;
SDL_Texture * text_texture;
SDL_Texture * inv_texture;
SDL_Surface * border_surface;
SDL_Surface * text_surface;
SDL_Surface * inv_surface;
SDL_Surface * free_image;
void image_array_init(IntroImageArray **array)
{
    *array = (IntroImageArray*)malloc(sizeof(IntroImageArray));
    (*array)->memory = NULL;
    (*array)->allocated = (*array)->used = (*array)->num_used = 0;
    (*array)->index = -1;
}
void image_array_push(IntroImageArray *array, void *data)
{
    size_t toallocate, size = sizeof(void*);
    if ((array->allocated - array->used) < size)
    {
        toallocate = array->allocated == 0 ? size : (array->allocated * 2);
        array->memory = realloc(array->memory, toallocate);
        array->allocated = toallocate;
    }
    array->memory[++array->index] = data;
    array->used = array->used + size;
    array->num_used += 1;
}
int image_array_length(IntroImageArray *array)
{
    return array->index + 1;
}
void image_array_insert(IntroImageArray *array, int index, void *data)
{
    if (index < 0 || index > array->index)
    {
        return;
    }
    array->memory[index] = data;
}
void image_array_free(IntroImageArray *array)
{
    for(int i  = 0; i < array->num_used; i++)
    {
        if(array->memory[i] != NULL)
        {
            SDL_FreeSurface(array->memory[i]);
        }
    }
    free(array->memory);
    free(array);
}
void* image_array_get(IntroImageArray *array, int index)
{
    if (index < 0 || index > array->index)
    {
        return NULL;
    }
    return array->memory[index];
}
void abort_game(const char* message)
{
    printf("%s \n", message);
    exit(1);
}
void free_map_speaker_and_item(){
    for(int i = 0; i < columns; i++)
    {
        for(int j = 0; j < rows; j++)
        {
            if(map[i][j].map_item)
            {
                free(map[i][j].map_item);
            }
            if(map[i][j].speaker)
            {
                free(map[i][j].speaker);
            }
        }
    }
}
void free_map_array(){
    for(int i = 0; i < columns; i++)
    {
        for(int j = 0; j < rows; j++)
        {
            if(map[i][j].N)
            {
                SDL_FreeSurface(map[i][j].N);
            }
            if(map[i][j].E)
            {
                SDL_FreeSurface(map[i][j].E);
            }
            if(map[i][j].S)
            {
                SDL_FreeSurface(map[i][j].S);
            }
            if(map[i][j].W)
            {
                SDL_FreeSurface(map[i][j].W);
            }
            if(map[i][j].interact_image)
            {
                SDL_FreeSurface(map[i][j].interact_image);
            }
        }
    }
}
void gameshutdown(void)
{
    if(p)
    {
        player_destruct(p);
    }
    if(map_filename)
    {
       free(map_filename);
    }
    if(border_texture)
    {
        SDL_DestroyTexture(border_texture);
    }
    if(text_texture)
    {
        SDL_DestroyTexture(text_texture);
    }
    if(texture)
    {
        SDL_DestroyTexture(texture);
    }
    if(border_surface)
    {
        SDL_FreeSurface(border_surface);
    }
    if(renderer){
        SDL_DestroyRenderer(renderer);
    }
    if(window)
    {
        SDL_DestroyWindow(window);
    }
    image_array_free(image_array);
    free_map_speaker_and_item();
    free_map_array();
    SDL_Quit();
}
player* player_constructor (int x, int y, char dir)
{
    struct player* instance = malloc (sizeof(player));
    if(instance != NULL)
    {
        instance->x = x;
        instance->y = y;
        instance->direction_facing = dir;
        instance->uniform = instance->pineapple = instance->endgame = instance->apple = 0;
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
void check_image_loaded(SDL_Surface *image)
{
    if(!image)
    {
        abort_game("Failed to load image");
    }
}
void check_texture_loaded(SDL_Texture * texture)
{
    if(!texture)
    {
        abort_game("Failed to load texture");
    }
}
void check_texture_and_destroy(SDL_Texture * texture)
{
    if(texture)
    {
        SDL_DestroyTexture(texture);
    }
}
bool check_true_or_false(char* ptr)
{
    if(strcmp(ptr,"true") == 0 )
    {
        return true;
    }
    else
    {
        return false;
    }
}
void check_image_null_or_load(SDL_Surface **image, char *ptr)
{
    if(strcmp(ptr,"NULL") == 0 )
    {
        image = NULL;
    }
    else
    {
        *image = SDL_LoadBMP(ptr);
        check_image_loaded(*image);
    }
}
void check_item_null_or_load(char *item, char *ptr){
    if(strcmp(ptr,"NULL") == 0 )
    {
        item = NULL;
    }
    else
    {
        item = malloc(strlen(ptr) + 1);
        strcpy(item, ptr);
    }
}
void check_min_num_lines_file(FILE* file)
{
    int line_count = 0;
    char line[256];
    while(fgets(line, sizeof(line), file))
    {
        line_count++;
    }
    if(line_count != 5)
    {
        abort_game("init.txt does not contain the correct number of lines");
    }
}
void check_char_is_num_in_range(char *pos)
{
    if (atoi(pos) == 0 && *pos != '0')
    {
        abort_game("Invalid starting co-ordinate");
    }
}
void clear_text()
{
    
    check_texture_and_destroy(text_texture);
    SDL_FreeSurface(text_surface);
    text_surface = SDL_LoadBMP("TEXT/TEXT_BOX_TEMPLATE.bmp");
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    check_texture_loaded(text_texture);
}
void set_text(char *text_file)
{
    check_texture_and_destroy(text_texture);
    SDL_FreeSurface(text_surface);
    text_surface = SDL_LoadBMP(text_file);
    if(!text_surface)
    {
        abort_game("Failed to load text image.");
    }
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    check_texture_loaded(text_texture);
}
void clear_inv()
{
    check_texture_and_destroy(inv_texture);
    SDL_FreeSurface(inv_surface);
    inv_surface = SDL_LoadBMP("INVENTORY/INV_BOX_TEMPLATE.bmp");
    inv_texture = SDL_CreateTextureFromSurface(renderer, inv_surface);
    check_texture_loaded(inv_texture);
}
void set_inv(char *inv_file)
{
    clear_inv();
    SDL_FreeSurface(inv_surface);
    inv_surface = SDL_LoadBMP(inv_file);
    inv_texture = SDL_CreateTextureFromSurface(renderer, inv_surface);
    check_texture_loaded(inv_texture);
}
void run_conversation(player *p)
{
    MapTile *tile = &map[player_get_position_x(p)][player_get_position_y(p)];

    if (strcmp(tile->speaker,"DOORGUARD1") == 0)
        {
            if(p->uniform == 0)
            {
                set_text("L1_DOOR_GUARD_1.bmp");
                tile->is_interactive = true;
            }
            else if(p->uniform == 1)
            {
                set_text("L1_DOOR_GUARD_2.bmp");
                tile->is_interactive = map[5][1].passable_from_W = true;
                if(tile->interact_image)
                {
                    SDL_FreeSurface(tile->interact_image);
                }
                tile->interact_image = tile->E = SDL_LoadBMP("ETC_L1/3x0_e_dooropen.bmp");
                free(tile->speaker);
                tile->speaker = malloc(strlen("NULL") + 1);
                strcpy(tile->speaker, "NULL");
            }
        }
        else if ((strcmp(tile->speaker,"DOORGUARD2") == 0) | (strcmp(tile->speaker,"DOORGUARD2_2") == 0))
        {
            if(p->pineapple== 0 && (strcmp(tile->speaker,"DOORGUARD2_2") == 0))
            {
                set_text("TEXT/L2_DOOR_GUARD/L2_DOOR_GUARD_2.bmp");
                tile->is_interactive = true;
            }
            if(p->pineapple== 0 && (strcmp(tile->speaker,"DOORGUARD2") == 0))
            {
                set_text("TEXT/L2_DOOR_GUARD/L2_DOOR_GUARD_1.bmp");
                tile->is_interactive = true;
                free(tile->speaker);
                tile->speaker = malloc(strlen("DOORGUARD2_2") + 1);
                strcpy(tile->speaker, "DOORGUARD2_2");
            }
            if(p->pineapple== 1)
            {
                set_text("TEXT/L2_DOOR_GUARD/L2_DOOR_GUARD_3.bmp");
                tile->is_interactive = map[4][2].passable_from_N = true;
                if(tile->interact_image)
                {
                    SDL_FreeSurface(tile->interact_image);
                }
                tile->interact_image = SDL_LoadBMP("ETC_L1/3x2_s.bmp");
                free(tile->speaker);
                tile->speaker = malloc(strlen("NULL") + 1);
                strcpy(tile->speaker, "NULL");
                SDL_FreeSurface(map[6][2].S);
                map[6][2].S = SDL_LoadBMP("ETC_L1/2x2_s.bmp");
                check_image_loaded(map[6][2].S);
                SDL_FreeSurface(map[7][2].S);
                map[7][2].S = SDL_LoadBMP("ETC_L1/1x2_s.bmp");
                check_image_loaded(map[7][2].S);
                SDL_FreeSurface(map[8][2].S);
                map[8][2].S = SDL_LoadBMP("ETC_L1/0x2_s.bmp");
                check_image_loaded(map[8][2].S);
            }
        }
        else if(strcmp(tile->speaker,"APPLEMACHINEBROKEN") == 0)
        {
            set_text("TEXT/TEXT_APPLE_MACHINE_1.bmp");
            tile->is_interactive =true;
        }
        else if(strcmp(tile->speaker,"MOFFICER") == 0)
        {
            set_text("TEXT/MOFFICER/TEXT_MOFFICER_1.bmp");
            tile->is_interactive = true;
            if(tile->interact_image)
            {
                SDL_FreeSurface(tile->interact_image);
            }
            SDL_FreeSurface(tile->S);
            tile->interact_image = tile->S = SDL_LoadBMP("ETC_L2/1x7_s.bmp");
            check_image_loaded(tile->interact_image);
            free(tile->speaker);
            tile->speaker = malloc(strlen("NULL") + 1);
            strcpy(tile->speaker, "NULL");
            SDL_FreeSurface(map[2][1].W);
            map[2][1].W = SDL_LoadBMP("ETC_L2/1x1_w_MOFFICER1.bmp");
            check_image_loaded(map[2][1].W);
            map[2][1].is_interactive = map[1][7].passable_from_N = true;
            if(map[2][1].interact_image != NULL)
            {
                SDL_FreeSurface(tile->interact_image);
            }
            map[2][1].interact_image = SDL_LoadBMP("ETC_L2/1x1_w_MOFFICER2.bmp");
            map[2][1].dir_need_to_face = 'W';
            map[2][0].passable_from_E = false;
        }
        else if(strcmp(tile->speaker,"MOFFICER2") == 0)
        {
            set_text("TEXT/MOFFICER/TEXT_MOFFICER_2.bmp");
            set_inv("INVENTORY/INV_BOX_UNIFORM.bmp");
            map[2][1].is_interactive = false;
            SDL_FreeSurface(map[2][1].W);
            map[2][1].W = SDL_LoadBMP("ETC_L2/1x1_w.bmp");
            p->pineapple = 0;
            SDL_FreeSurface(map[2][0].N);
            map[2][0].N = SDL_LoadBMP("ETC_L2/1x0_n_2.bmp");
            map[2][0].is_interactive = map[2][0].passable_from_E = true;
            free(map[2][0].speaker);
            map[2][0].speaker = malloc(strlen("NULL") + 1);
            strcpy(map[2][0].speaker, "NULL");
            free(map[2][0].map_item);
            map[2][0].map_item = malloc(strlen("APPLE") + 1);
            strcpy(map[2][0].map_item, "APPLE");
        }
        else if((strcmp(tile->speaker,"DOORGUARD3") == 0) | (strcmp(tile->speaker,"DOORGUARD3_2") == 0))
        {
            if(p->apple== 0 && (strcmp(tile->speaker,"DOORGUARD3_2") == 0))
            {
                set_text("TEXT/L3_DOOR_GUARD/L3_DOOR_GUARD_2.bmp");
                tile->is_interactive = true;
            }
            if(p->apple== 0 && (strcmp(tile->speaker,"DOORGUARD3") == 0))
            {
                set_text("TEXT/L3_DOOR_GUARD/L3_DOOR_GUARD_1.bmp");
                tile->is_interactive = true;
                free(tile->speaker);
                tile->speaker = malloc(strlen("DOORGUARD3_2") + 1);
                strcpy(tile->speaker, "DOORGUARD3_2");
            }
            if(p->apple == 1)
            {
                set_text("TEXT/L3_DOOR_GUARD/L3_DOOR_GUARD_3.bmp");
                p->apple = 0;
                p->endgame = 1;
                set_inv("INVENTORY/INV_BOX_UNIFORM.bmp");
                SDL_FreeSurface(tile->S);
                tile->S = SDL_LoadBMP("ETC_L2/3x3_s_EYE.bmp");
                free(tile->speaker);
                tile->speaker = malloc(strlen("NULL") + 1);
                strcpy(tile->speaker, "NULL");
            }
        }
        else if (strcmp(tile->speaker,"NULL") == 0)
        {
            tile->is_interactive = false;
        }
}
void get_item(player *p, char *item)
{
    if(strcmp(item,"NULL") == 0)
    {
        return;
    }
    else if(strcmp(item,"UNIFORM") == 0)
    {
        p->uniform = 1;
        set_inv("INVENTORY/INV_BOX_UNIFORM.bmp");
        set_text("TEXT/TEXT_ITEM_UNIFORM.bmp");
    }
    else if(strcmp(item,"PINEAPPLE") == 0)
    {
        p->pineapple = 1;
        set_inv("INVENTORY/INV_BOX_PINEAPPLE.bmp");
        set_text("TEXT/TEXT_ITEM_PINEAPPLE.bmp");
        map[8][2].passable_from_S = true;
        SDL_FreeSurface(map[7][2].N);
        map[7][2].N = SDL_LoadBMP("ETC_L1/1x2_n.bmp");
        SDL_FreeSurface(map[6][2].N);
        map[6][2].N = SDL_LoadBMP("ETC_L1/2x2_n.bmp");
        SDL_FreeSurface(map[5][2].N);
        map[5][2].N = SDL_LoadBMP("ETC_L1/3x2_n.bmp");
    }
    else if(strcmp(item,"APPLE") == 0)
    {
        p->apple = 1;
        set_inv("INVENTORY/INV_BOX_APPLE.bmp");
        set_text("TEXT/TEXT_ITEM_APPLE.bmp");
    }
    return;
}
void read_line_to_map(char *s, int x, int y)
{
    char delim[] = " ";

    char *ptr = strtok(s, delim);
    int counter = 0;
    while(ptr != NULL)
    {
        switch(counter)
        {
            case 0:
                check_image_null_or_load(&map[x][y].N, ptr);
                break;
            case 1:
                check_image_null_or_load(&map[x][y].E, ptr);
                break;
            case 2:
                check_image_null_or_load(&map[x][y].S, ptr);
                break;
            case 3:
                check_image_null_or_load(&map[x][y].W, ptr);
                break;
            case 4:
                map[x][y].passable_from_N = check_true_or_false(ptr);
                break;
            case 5:
                map[x][y].passable_from_E = check_true_or_false(ptr);
                break;
            case 6:
                map[x][y].passable_from_S = check_true_or_false(ptr);
                break;
            case 7:
                map[x][y].passable_from_W = check_true_or_false(ptr);
                break;
            case 8:
                map[x][y].is_interactive = check_true_or_false(ptr);
                break;
            case 9:
                check_image_null_or_load(&map[x][y].interact_image, ptr);
                break;
            case 10:
                if(strcmp(ptr, "(char)0") == 0)
                {
                    map[x][y].dir_need_to_face = (char)0;
                }else
                {
                    map[x][y].dir_need_to_face = *ptr;
                }
                break;
            case 11:
                map[x][y].map_item = malloc(strlen(ptr) + 1);
                strcpy(map[x][y].map_item, ptr);
                break;
            case 12:
                map[x][y].speaker = malloc(strlen(ptr) + 1);
                strcpy(map[x][y].speaker, ptr);
                break;
            default: break;
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
        abort_game("Failed to load map.txt file");
    }
    char line[256];
    for(int i = 0; i < columns; i++)
    {
        for(int j = 0; j < rows; j++)
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
    load_map_file(map_filename);
}
void restart_game(){
    free_map_speaker_and_item();
    free_map_array();
    clear_text();
    clear_inv();
    init_map();
    player_set_position(p, 0, 0);
    set_text("TEXT/TEXT_WAKE_UP.bmp");
}
void get_user_input(SDL_Event event)
{
    if(event.type == SDL_KEYDOWN)
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_UP:
                if(num_intro_screens > 0)
                {
                    break;
                }
                else if (player_get_direction_facing(p) == 'N' && map[player_get_position_x(p) + 1][player_get_position_y(p)].passable_from_S && (player_get_position_x(p) + 1) < columns)
                {
                    player_set_position(p, p->x + 1, p->y);
                    key[KEY_UP] = redraw = true;
                }
                else if (player_get_direction_facing(p) == 'S' && map[player_get_position_x(p) - 1][player_get_position_y(p)].passable_from_N && (player_get_position_x(p) - 1 ) >= 0)
                {
                    player_set_position(p, p->x - 1, p->y);
                    key[KEY_UP] = redraw = true;
                }
                else if(player_get_direction_facing(p) == 'E' && map[player_get_position_x(p)][player_get_position_y(p) + 1].passable_from_W && (player_get_position_y(p) + 1) < rows)
                {
                    player_set_position(p, p->x, p->y+1);
                    key[KEY_UP] = redraw = true;
                }
                else if(player_get_direction_facing(p) == 'W' && map[player_get_position_x(p)][player_get_position_y(p) - 1].passable_from_E &&(player_get_position_y(p) - 1) >= 0)
                {
                    player_set_position(p, p->x, p->y-1);
                    key[KEY_UP] = redraw = true;
                }
                else if(player_get_position_x(p) == 0 && player_get_position_y(p) == 3 && player_get_direction_facing(p) == 'S' && p->endgame == 1)
                {
                    restart_game();
                    key[KEY_UP] = redraw = true;
                }
                break;
            case SDLK_LEFT:
                if(num_intro_screens > 0)
                {
                    break;
                }
                else if (player_get_direction_facing(p) == 'N')
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
                key[KEY_LEFT] = redraw = true;
                break;
            case SDLK_RIGHT:
                if(num_intro_screens > 0)
                {
                    break;
                }
                else if (player_get_direction_facing(p) == 'N')
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
                key[KEY_RIGHT] = redraw = true;
                break;
            case SDLK_ESCAPE:
                break;
            case SDLK_SPACE:
                if(player_get_position_x(p) == 4 && player_get_position_y(p) == 2)
                {
                    player_set_position(p, 2, 3);
                    player_set_direction(p, 'N');
                }
                if(player_get_position_x(p) == 0 && player_get_position_y(p) == 0)
                {
                    player_set_position(p, 7, 0);
                    player_set_direction(p, 'S');
                    set_text("TEXT/TEXT_BOX_TEST.bmp");
                }
                key[KEY_SPACE] = redraw = true;
                break;
            default:
                redraw = false;
                break;
        }
    }
}
void graphics_show_direction_facing(){
    check_texture_and_destroy(texture);
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
    check_texture_loaded(texture);
}
void graphics_render_multiple_texture()
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, &view_rect);
    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
    SDL_RenderCopy(renderer, inv_texture, NULL, &inv_rect);
    SDL_RenderCopy(renderer, border_texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
void update_graphics()
{
    if(num_intro_screens > 1 && key[KEY_SPACE])
    {
        key[KEY_SPACE] = false;
        check_texture_and_destroy(texture);
        texture = SDL_CreateTextureFromSurface(renderer, image_array_get(image_array, array_pos));
        check_texture_loaded(texture);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        num_intro_screens--;
        array_pos++;
        return;
    }
    else if(num_intro_screens > 0 && key[KEY_SPACE])
    {
        key[KEY_SPACE] = false;
        graphics_show_direction_facing();
        graphics_render_multiple_texture();
        num_intro_screens--;
        return;
    }
    else if(key[KEY_SPACE])
    {
        key[KEY_SPACE] = false;
        if(player_get_position_x(p) != 0 && player_get_position_y(p) != 0){
            clear_text();
        }
        if(map[player_get_position_x(p)][player_get_position_y(p)].is_interactive && player_get_direction_facing(p) == map[player_get_position_x(p)][player_get_position_y(p)].dir_need_to_face)
        {
            check_texture_and_destroy(texture);
            texture = SDL_CreateTextureFromSurface(renderer, map[player_get_position_x(p)][player_get_position_y(p)].interact_image);
            map[player_get_position_x(p)][player_get_position_y(p)].is_interactive = false;
            switch(player_get_direction_facing(p)){
                case 'N': SDL_BlitSurface(map[player_get_position_x(p)][player_get_position_y(p)].N,NULL,map[player_get_position_x(p)][player_get_position_y(p)].interact_image,NULL);
                    break;
                case 'E': SDL_BlitSurface(map[player_get_position_x(p)][player_get_position_y(p)].E,NULL,map[player_get_position_x(p)][player_get_position_y(p)].interact_image,NULL);
                    break;
                case 'S':
                    SDL_BlitSurface( map[player_get_position_x(p)][player_get_position_y(p)].S,NULL,map[player_get_position_x(p)][player_get_position_y(p)].interact_image,NULL);
                    break;
                case 'W': SDL_BlitSurface(map[player_get_position_x(p)][player_get_position_y(p)].W,NULL,map[player_get_position_x(p)][player_get_position_y(p)].interact_image,NULL);
                    break;
                default:break;
            }
            get_item(p, map[player_get_position_x(p)][player_get_position_y(p)].map_item);
            run_conversation(p);
            graphics_render_multiple_texture();
            return;
        }
        else
        {
            graphics_show_direction_facing();
            graphics_render_multiple_texture();
            return;
        }
    }
    else if(key[KEY_RIGHT] || key[KEY_LEFT] || key[KEY_UP])
    {
        if(num_intro_screens > 0)
        {
            key[KEY_RIGHT] = key[KEY_LEFT] = key[KEY_UP] = key[KEY_DOWN] = false;
            return;
        }
        else if(player_get_position_y(p) != 0 && player_get_position_x(p) != 0)
        {
            clear_text();
        }
        key[KEY_RIGHT] = key[KEY_LEFT] = key[KEY_DOWN] = key[KEY_DOWN] = false;
        graphics_show_direction_facing();
        graphics_render_multiple_texture();
        return;
    }
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
            done = true;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
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
void init_player(int x, int y, char dir)
{
    p = player_constructor(x, y, dir);
}
void load_starting_images_from_line(char *line)
{
    if (line && !line[0])
    {
        return;
    }
    char delim[] = " ";
    char *ptr = strtok(line, delim);
    while(ptr != NULL)
    {
        SDL_Surface *image = SDL_LoadBMP(ptr);
        check_image_loaded(image);
        image_array_push(image_array, image);
        num_intro_screens++;
        ptr = strtok(NULL, delim);
    }
}
void load_starting_game_vars(int *x, int *y, char *dir)
{
    FILE* file = fopen("init.txt", "r");
    if(file == NULL)
    {
        abort_game("Failed to load init.txt file");
    }
    image_array_init(&image_array);
    check_min_num_lines_file(file);
    rewind(file);
    char line[256];
    int line_number = 1;
    while(fgets(line, sizeof(line), file))
    {
        char* pos = strrchr(line, '\n');
        if (pos) *pos = 0;
        switch(line_number)
        {
            case 1:
                map_filename = (char *)calloc(1, strlen(line) + 1);
                if(map_filename == NULL)
                {
                    abort_game("Failed to allocate memory to map filename");
                }
                strcpy(map_filename, line);
                break;
            case 2:
                load_starting_images_from_line(line);
                break;
            case 3:
                check_char_is_num_in_range(&line[0]);
                *x = atoi(line);
                break;
            case 4:
                check_char_is_num_in_range(&line[0]);
                *y = atoi(line);
                break;
            case 5:
                *dir = line[0];
                break;
            default: break;
        }
        line_number++;
    }
    fclose(file);
}
void init(void)
{
    int x, y;
    char dir;
    load_starting_game_vars(&x, &y, &dir);
    init_map();
    init_player(x, y, dir);
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Escape the Clocktower", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    if(!window)
    {
        abort_game("Failed to create SDL window");
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer)
    {
        abort_game("Failed to create SDL renderer");
    }
    border_surface = SDL_LoadBMP("FULL_FRAME_BORDER.bmp");
    if(!border_surface)
    {
        abort_game("Failed to load FULL_FRAME_BORDER.bmp");
    }
    set_text("TEXT/TEXT_BOX_TEST.bmp");
    set_inv("INVENTORY/INV_BOX_TEMPLATE.bmp");
    border_texture = SDL_CreateTextureFromSurface(renderer, border_surface);
    if(!border_texture)
    {
        abort_game("Failed to load border_texture");
    }
    if(image_array_length(image_array) == 0)
    {
        graphics_show_direction_facing();
        graphics_render_multiple_texture();
    }
    else
    {
        texture = SDL_CreateTextureFromSurface(renderer, image_array_get(image_array, array_pos));
        check_texture_loaded(texture);
        array_pos++;
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
}
int main(int argc, char ** argv)
{
    init();
    game_loop();
    gameshutdown();
    exit(0);
}
>>>>>>> d51be4f5ec23d48e4308dd3bf26e0b2bef48e809

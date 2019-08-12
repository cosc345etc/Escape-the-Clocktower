Hi 

It's great to have you join the project. We'll run you through the changes since are alpha release.

  - EtC Team

# Overview
Escape the Clocktower is a first-person dungeon crawler/escape the room game, with the theme of escaping the Otago University Clocktower. 

Our coding standards are Allman layout, self-explanatory variable and method names, sensible structuring. 

# Installing EtC
* Clone repository https://github.com/cosc345etc/Escape-the-Clocktower.git
* Open xcode file /Application/EtC_Beta/EtC_Beta.xcodeproj
* Run Build (command B)
* Run (command R)


# Changelog For Beta Release

* main.c re-written completely from the ground up
* Now useing the SDL2 as opposed to the Allegro library for image display and keyboard input
* Code, frameworks, and dependencies altered to work as an XCode project
* Added an additional dimension of movement (E ← → W)
* Maps are now loaded from external text files
* Map cells are now passable from particular directions, allowing for un-passable walls (as opposed to unpassable cells), as well as one-way walls
* Map cells now have an “interactable” property allowing the player to change views at a particular direction
* Teleportation is possible between two points of the map
* Multiple levels implemented
* Updated title screen
* 100+ new image files

# Bug Fixes

* Fixed Xcode “code signing” error
* Map no longer generates upside-down (nor back-to-front)
* Fixed a bug where the player could get trapped by invisible walls 
* Passable map cells are now passable from the correct east/west dimension
* Interactable objects no longer alter only the north view

# Data Structures
The two main data structures we've created are the map and the player.

## Map
The map is a two dimensional arrays of MapTile structs.

A MapTile struct contains pointers *N, *E, *S, *W to the images that are displayed when the player is facing north, east, south and west. The same images can be used by multiple tiles, and can be NULL. A MapTile struct also contains a boolean value passable to indicate whether the player can enter that tile or not. Impassable tiles will usually have null image pointers, but when we implement unlocking new areas we intend to initialise the tile with the images it will use, and only alter the passable value from false to true.

We intend to implement items and NPCs as a separate data type that will contain the necessary information within itself; the only change to the map tiles is expected to be adding a single pointer to the object contained in that tile, or nothing.

## Player
The player structure contains the position of the player in the world: two integers for the x and y coordinates on the map, and a char that should contain 'N', 'E', 'S' or 'W' to indicate the direction the player is facing.

We intend to implement an inventory system and flags; those are expected to be part of this struct in the final version of the game.

## Keys enum
MYKEYS enumerates the codes that are later used to distinguish pressing the up, down, left and right arrow keys or the spacebar.

## Other global variables
done tracks whether the game is finished. It is a boolean value, initialised to false when the game begins, that is set to true when the user presses escape or closes the game window.redraw tracks whether the graphics need to be updated. It is a boolean value, initialised to true when the gameloop begins. It is set to false if nothing happens that requires a graphics update; is confirmed as true if  
key tracks what key has been pressed. It is a pointer to an array of boolean values, initialised to false when the array is declared. The entries in the array are to be accessed using the MYKEYS enum for indices.
first distinguishes the first keypress from the user. It is a boolean value, initialised to true when the variable is declared. While it is true, all keypresses other than 'space' are ignored; when the first 'space' is entered to move past the title screen, this variable is set to false.

p is the pointer to the player struct. It is initialised by the method init_player.

# Methods

## Changed/Updated Methods
 
* **init(void)** - No longer using Allegro 
* **init_map(void)** - No longer hard coded
* **game_loop(void)** - No longer using Allegro 
* **shutdown == game_shutdown** - No longer using Allegro
* **get_user_input** - Down is fixed and moving east & west fixed
* **update_graphics** - No longer using Allegro

## New Methods

*  **player_destruct** (player* this) -  
*  **graphics_show_direction_facing** - Shows players current direction and calls DL_CreateTextureFromSurface (SDL function to create a texture from an existing surface) and calls player_get_position
*  ***check_true_or_false(char* ptr)*** - checks conditions of N,E,S,W and returns boolean value 
*  ***read_line_to_map(char *s, int x, int y)*** - reads the .bmp files and calls check_true_or_false
*  ***load_map_file(char *fname)*** - loads a .bmp map file and then calls read_line_to_map

## Exisitng Methods

**abort_game** 
ends the game in case of error. It takes an error message as a parameter, prints that message, and exits the program.

## Main
The main method has the standard C parameters. It calls five methods, none of which take parameters or return values.

**init** 
sets up SDL2 for the session. It initialises the assets from SDSL2: the library itself, keyboard, timer, image display, a bitmap of our title screen and event queue. It checks that these have succeeded, and calls abort_game if not.

**init_map** 
creates the map from Reading in data from an .bmp file

**init_player** 
initialises the pointer p. It calls player_constructor to create the player struct, and sets the global variable p to point to that struct.

**game_loop** 
runs the game. While the game is not finished [done] it uses the SDL2 event queue to watch for input. Escape or closing the game window end the game. Otherwise, it calls get_user_input to handle user input. Then it calls update_graphics to redraw the game display if necessary.

**shutdown** 
closes the game. It calls functions to destroy the SDL2 objects, and the player struct.

## Player Methods
**player_get_position_x**,**player_get_position_y** and **player_get_direction_facing** 
take the player struct as a parameter, and return the x coordinate, y coordinate and current direction.

**player_set_position** 
takes the player struct and two integer coordinates as parameters. Sets the player's new coordinates to the input coordinates.

**player_set_direction** 
takes the player struct and a single character direction as parameters. Sets the player's new direction to the input direction.

**player_constructor** 
makes the player at the starting point of the map. It allocates memory for the player struct, initialises the position to 0, 0, facing N, and returns a pointer to this struct.

**player_destruct** 
takes player struct as a parameter, frees it. Currently this takes one command but if the struct is extended to include other allocations, the corresponding frees will go here.

## Gameplay Methods
**get_user_input** 
takes the keypress event as a parameter and checks what key was pressed. It has no return value. The first keypress must be 'space'. 'Up', 'left' and 'right' cause the relevant value in the keys[] array to be set to true. 

If up arrow key: update the player location by +1 if facing North, or -1 if facing South, after checking is that tile is passable. Moving East or West is currently disabled. 

If left or right arrow key: update the direction that the player is facing, counterclockwise or clockwise respectively. 
All other keypresses print an error message and do not require the graphics to update.

**graphics_update** 
changes the graphics to reflect the player's actions. It is called by the gameplay loop after _get_user_input_, and depends on that method, but through the global variables first and keys[]: it does not take parameters or return values.

If 'space' was pressed and it's the first keypress, or if it's not the first keypress and 'up', 'left' or 'right' was pressed, then the method checks the current location and direction of the player and feeds the relevant image into SDL2 bitmap and display functions to make it show up.





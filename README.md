

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

# Methods

## Changed/Updated Methods
 
* **init(void)** - No longer using Allegro 
* **init_map(void)** - No longer hard coded
* **game_loop(void)** - No longer using Allegro 
* **shutdown == game_shutdown** - No longer using Allegro
* **get_user_input** - Down is fixed and moving east & west fixed
* **update_graphics** - No longer using Allegro

## New Methods

*  **player_destruct** (player* this) - frees allocated memory 
*  **graphics_show_direction_facing** - Shows players current direction and calls DL_CreateTextureFromSurface (SDL function to create a texture from an existing surface) and calls player_get_position
*  ***check_true_or_false(char* ptr)*** - checks conditions of N,E,S,W and if passable, returns boolean value 
*  ***read_line_to_map(char *s, int x, int y)*** - reads the .bmp files in and calls check_true_or_false
*  ***load_map_file(char *fname)*** - loads a .bmp map file and then calls read_line_to_map

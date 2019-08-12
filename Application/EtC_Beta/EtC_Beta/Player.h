#ifndef UNTITLED2_PLAYER_H
#define UNTITLED2_PLAYER_H

typedef struct player player;

player* player_constructor (void);

void     player_set_position (player* this, int x, int y);
void     player_set_direction (player* this, char d);
int      player_get_position_x (const player* this);
int      player_get_position_y (const player* this);
char     player_get_direction_facing(const player* this);
void     player_destruct (player* this);


#endif
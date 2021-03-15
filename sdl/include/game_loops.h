/*
** ETNA PROJECT, 09/10/2019 by places_m
** my.h
** File description:
**      header file
*/
#ifndef __LOOPS_H__
#define __LOOPS_H__

#include "./game.h"

// Menu

void update_game_solo_events(stGame *game, SDL_Event event);
void update_game_solo_data(stGame *game);
void draw_game(stGame *game);
BOOL game_loop_init(stGame *game);
char *random_game_music();
void game_loop_solo(stGame *game);

#endif

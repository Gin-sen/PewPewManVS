/*
** ETNA PROJECT, 09/10/2019 by places_m
** my.h
** File description:
**      header file
*/
#ifndef __MENU_H__
#define __MENU_H__

#include "./game.h"

// Menu

void update_menu_events(stGame *game, SDL_Event event, Mix_Chunk **bip);
void draw_menu(stGame *game);
BOOL menu_init(stGame *game);
char *random_menu_music();
void menu(stGame *game);

#endif

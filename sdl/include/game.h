/*
** ETNA PROJECT, 09/10/2019 by places_m
** my.h
** File description:
**      header file
*/

#ifndef __GAME_H__
#define __GAME_H__

// Std libs
#ifndef unistd
#define unistd
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#endif

// SDL libs
#ifndef __SDL2_H__
#define __SDL2_H__
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#endif

// Constantes
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

// Definitions
#ifndef __BOOL__
#define __BOOL__

typedef enum {FALSE, TRUE} BOOL;

#endif


// Game

typedef struct stGame {
    // Variables de la SDL
    SDL_Point screenSize;
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;

    // Textures
        // Menu
    SDL_Texture *pTextBackground;
    SDL_Texture *pTextCursor;
    SDL_Texture *pTextMenu[4];
    SDL_Texture *pTextMenuBold[4];
        // Game

    // Fonts
    TTF_Font *pFont;

    // Variables de Logique
    BOOL should_run;
    int game_mode;
    BOOL actionKeyPressed;
    int input_delay;

    // Variables de données
        // Menu
    char *text_menu[4];
        // Game


} stGame;

stGame *game_init();
void game_destroy(stGame *game);
#endif

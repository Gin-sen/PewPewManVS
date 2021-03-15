/*
** ETNA PROJECT, 09/10/2019 by places_m
** my.h
** File description:
**      header file
*/

#ifndef __MYHEADER_H__
#define __MYHEADER_H__

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


// Custom headers
#include "./menu.h"
//#include "./character.h"
//#include "./bombs.h"

#ifndef __CONSTS__
#define __CONSTS__

// Constantes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

#endif




#endif


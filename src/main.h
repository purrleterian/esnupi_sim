
#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_main.h>
#include <time.h>


typedef struct {float x, y;} Vec2;

#define SDL_FLAGS SDL_INIT_VIDEO

#define WINDOW_TITLE "Esnupi"
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600


#define PLAYER_SCALE_N 12
#define PLAYER_VEL 5
#define PLAYER_ACC 1
#define GRAVITY 0.8

#endif


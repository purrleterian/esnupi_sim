#ifndef WOODSTOCK_H
#define WOODSTOCK_H

#include "main.h"
#include "player.h"
#include "house.h"

typedef struct {
    SDL_Renderer *renderer;
    SDL_Texture *image;
    SDL_FRect rect;

    Vec2 accel;
    Vec2 vel;

    const bool *keystate;

    bool facing_right;
} Woodstock;

bool ws_new(Woodstock **bird, SDL_Renderer *renderer);
void ws_free(Woodstock **bird);
void ws_update(Woodstock *ws, Player *p, House *h);
void ws_draw(Woodstock *ws);

#endif

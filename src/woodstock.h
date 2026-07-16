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

    Vec2 target;

    Player *player;
    House *house;
    bool dir;
    bool sitting;
} Woodstock;

bool ws_new(Woodstock **bird, SDL_Renderer *renderer, Player *player, House *house);
void ws_free(Woodstock **bird);
void ws_update(Woodstock *ws);
void ws_draw(Woodstock *ws);
void land_bird(Woodstock *ws);

#endif

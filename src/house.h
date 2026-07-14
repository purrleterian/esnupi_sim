#ifndef HOUSE_H
#define HOUSE_H

#include "ground.h"
#include "main.h"

typedef struct {
    SDL_Renderer *renderer;
    SDL_Texture *image;
    SDL_FRect rect;

    float tile_w;
    float tile_h;

} House;


bool house_new(House **hosue, SDL_Renderer *renderer);
void house_free(House **house);
void house_update(House *h, GroundBlock *ground);
void house_draw(const House *house);

#endif

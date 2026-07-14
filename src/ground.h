#ifndef GROUND_H
#define GROUND_H


#include "main.h"
#include "player.h"


typedef struct {
    SDL_Renderer *renderer;
    SDL_Texture *image;
    SDL_FRect *rects;

    float tile_w;
    float tile_h;
    int count;

    int *sprite_numbers;
    bool *flipped_sprites;
} GrassLeaf;


typedef struct {
    SDL_Renderer *renderer;
    SDL_Texture *image;
    SDL_FRect *rects;

    float tile_w;
    float tile_h;

    int count;

    GrassLeaf *leaf;
} GroundBlock;

bool ground_new(GroundBlock **ground, SDL_Renderer *renderer);
void ground_free(GroundBlock **ground);
void ground_update(GroundBlock *ground, Player *p);
void ground_draw(const GroundBlock *ground);

#endif

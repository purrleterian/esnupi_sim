#ifndef PLAYER_H
#define PLAYER_H

#include "main.h"
#include "ground.h"

typedef enum {
    WALKING,
    JUMPING,
    CROUCHING,
    STANDING,
    SLEEPING,
    SITTING
} AnimationState;

typedef struct {
    SDL_Renderer *renderer;
    SDL_Texture *image;
    SDL_FRect rect;
    const bool *keystate;

    Vec2 accel;
    Vec2 vel;
   
    bool is_jumping;
    bool facing_right;
  
    AnimationState state;
} Player;



bool player_new(Player **player, SDL_Renderer *renderer);
void player_free(Player **player);
void player_update(Player *p, GroundBlock *ground);
void player_draw(const Player *p);


#endif

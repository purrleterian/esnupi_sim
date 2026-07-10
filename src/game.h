#ifndef GAME_H
#define GAME_H

#include "ground.h"
#include "main.h"
#include "player.h"

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    Player *player;
    GroundBlock *ground;
    bool is_running;

} Game;

void game_free(Game **game);
bool game_setup(Game **game);
void game_run(Game *g);

#endif

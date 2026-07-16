#include "game.h"
#include "init_sdl.h"
#include "load_media.h"

void game_events(Game *g);
void game_draw(Game *g);
void game_update(Game *g);



bool game_setup(Game **game) {
    *game = calloc(1, sizeof(Game));
    if (*game == NULL) {
        fprintf(stderr, "Erro while allocating memory (calloc game setup): %s\n", SDL_GetError());
    }

    Game *g = *game;

    if (!game_init_sdl(g)) return false;
    if (!game_load_media(g)) return false;
    if (!player_new(&g->player, g->renderer)) return false;
    if (!ground_new(&g->ground, g->renderer)) return false;
    if (!house_new(&g->house, g->renderer)) return false;
    if (!ws_new(&g->bird, g->renderer, g->player, g->house)) return false;

    g->is_running = true;
    return true;
}


void game_free(Game **game) {
    if (*game) {
        Game *g = *game;

        if (g->player) {
            player_free(&g->player);
        }

        if (g->ground) {
            ground_free(&g->ground);
        }

        if (g->house) {
            house_free(&g->house);
        }

        if (g->bird) {
            ws_free(&g->bird);
        }

        if (g->window) {
            SDL_DestroyWindow(g->window);
            g->window = NULL;
        }

        if (g->renderer) {
            SDL_DestroyRenderer(g->renderer);
            g->renderer = NULL;
        }
    
        SDL_Quit();
        
        free(g);
        g = NULL;
        *game = NULL;
    }
}

void game_events(Game *g) {
    while (SDL_PollEvent(&g->event)) {
        switch (g->event.type) {
            case SDL_EVENT_QUIT:
                g->is_running = false;
                break;

            case SDL_EVENT_KEY_DOWN:
                switch(g->event.key.scancode) {
                    case SDL_SCANCODE_ESCAPE:
                        g->is_running = false;
                        break;
                    default: break;
                }
                break;

            case SDL_EVENT_KEY_UP:
                switch(g->event.key.scancode) {
                    case SDL_SCANCODE_U:
                        g->bird->sitting = !g->bird->sitting;
                        break;

                    default: break;
                }

            default: break;
        }
    }
}

void game_update(Game *g) {
    player_update(g->player);
    ground_update(g->ground, g->player);
    house_update(g->house, g->ground, g->player);
    ws_update(g->bird);

}
    
void game_draw(Game *g) {
    SDL_SetRenderDrawColor(g->renderer, 139, 208, 186, 255);

    SDL_RenderClear(g->renderer);
    
    ground_draw(g->ground);
    house_draw(g->house);
    player_draw(g->player);
    ws_draw(g->bird);
    SDL_RenderPresent(g->renderer);

}

void game_run(Game *g) {
    while (g->is_running) {
        game_events(g);

        game_update(g);
        game_draw(g);
        SDL_Delay(16);
        
    }
}

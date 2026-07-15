#include "woodstock.h"

bool ws_new(Woodstock **bird, SDL_Renderer *renderer) {
    *bird = calloc(1, sizeof(Woodstock));
    if (*bird == NULL) {
        fprintf(stderr, "Error while allocating memory for woodstock: %s\n", SDL_GetError());
        return false;
    }


    printf("Woodstock born.\n");
    return true;
}

void ws_free(Woodstock **bird) {
    if (*bird) {
        Woodstock *ws = *bird;
        if (ws->image) {
            SDL_DestroyTexture(ws->image);
            ws->image = NULL;
        }

        ws->renderer = NULL;
        free(ws);
        ws = NULL;
        *bird = NULL;

        printf("Woodstock Free.\n");
    }
}

void ws_update(Woodstock *ws, Player *p) {

}

void ws_draw(Woodstock *ws) {
    SDL_RenderTextureRotated(ws->renderer, ws->image, NULL, &ws->rect, 0, NULL, !ws->facing_right ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}
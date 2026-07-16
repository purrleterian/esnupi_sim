#include "woodstock.h"

bool ws_new(Woodstock **bird, SDL_Renderer *renderer) {
    *bird = calloc(1, sizeof(Woodstock));
    if (*bird == NULL) {
        fprintf(stderr, "Error while allocating memory for woodstock: %s\n",
                SDL_GetError());
        return false;
    }

    Woodstock *ws = *bird;
    ws->renderer = renderer;

    ws->image = IMG_LoadTexture(ws->renderer, "./res/woodstock.png");
    if (!ws->image) {
        fprintf(stderr, "Error while opening texture file: %s\n",
                SDL_GetError());
        return false;
    }

    if (!SDL_SetTextureScaleMode(ws->image, SDL_SCALEMODE_NEAREST)) {
        fprintf(stderr, "Error while setting scale mode: %s\n", SDL_GetError());
        return false;
    }



    if (!SDL_GetTextureSize(ws->image, &ws->rect.w, &ws->rect.h)) {
        fprintf(stderr, "Error while setting rect size: %s\n", SDL_GetError());
        return false;
    }

    ws->rect.w *= PLAYER_SCALE_N;
    ws->rect.h *= PLAYER_SCALE_N;

    ws->facing_right = true;

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
    int x_offset = 8;
    int y_offset = 8;
    ws->rect.x = p->rect.x + p->rect.w + x_offset;
    ws->rect.y = p->rect.y - ws->rect.h - y_offset;

}

void ws_draw(Woodstock *ws) {
    SDL_RenderTextureRotated(ws->renderer, ws->image, NULL, &ws->rect, 0, NULL,
                             !ws->facing_right ? SDL_FLIP_HORIZONTAL
                                               : SDL_FLIP_NONE);
}

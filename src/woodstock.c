#include "woodstock.h"
#include "main.h"

bool ws_new(Woodstock **bird, SDL_Renderer *renderer, Player *player,
            House *house) {
    *bird = calloc(1, sizeof(Woodstock));
    if (*bird == NULL) {
        fprintf(stderr, "Error while allocating memory for woodstock: %s\n",
                SDL_GetError());
        return false;
    }

    Woodstock *ws = *bird;
    ws->renderer = renderer;
    ws->player = player;
    ws->house = house;

    ws->image = IMG_LoadTexture(ws->renderer, "./res/woodstock_min.png");
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

    ws->dir = true;

    ws->keystate = SDL_GetKeyboardState(NULL);

    ws->sitting = false;

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

        ws->player = NULL;
        ws->house = NULL;

        ws->renderer = NULL;
        free(ws);
        ws = NULL;
        *bird = NULL;

        printf("Woodstock Free.\n");
    }
}

void ws_update(Woodstock *ws) {
    int x_offset = 2 * PLAYER_SCALE_N;
    int y_offset = 0;

    float spring_strength = 0.045;

    ws->dir = ws->player->facing_right;

    // targets for where the bird SHOULD be;

    if (ws->dir) {
        ws->target =
            (Vec2){.x = ws->player->rect.x + ws->player->rect.w + x_offset,
                   .y = ws->player->rect.y - ws->rect.h - y_offset};
    } else {
        ws->target = (Vec2){.x = ws->player->rect.x - ws->rect.w - x_offset,
                            .y = ws->player->rect.y - ws->rect.h - y_offset};
    }

    if (ws->sitting) {
        land_bird(ws);
    }

    // calculate how far the bird is from where he should be
    Vec2 dist_from_target = {.x = ws->target.x - ws->rect.x,
                             .y = ws->target.y - ws->rect.y};

    // further from the target = faster it goes back, rubber band
    ws->accel.x = dist_from_target.x * spring_strength;
    ws->accel.y = dist_from_target.y * spring_strength;

    ws->accel.x -= ws->vel.x * 0.5;
    ws->accel.y -= ws->vel.y * 0.5;

    ws->vel.x += ws->accel.x;
    ws->vel.y += ws->accel.y;

    ws->rect.x += ws->vel.x + 0.5 * ws->accel.x;
    ws->rect.y += ws->vel.y + 0.5 * ws->accel.y;
}

void ws_draw(Woodstock *ws) {
    SDL_RenderTextureRotated(ws->renderer, ws->image, NULL, &ws->rect, 0, NULL,
                             ws->dir ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void land_bird(Woodstock *ws) {
    ws->dir =
        ws->player->rect.x > ((WINDOW_WIDTH / 2.0f) - ws->player->rect.w / 2)
            ? false
            : true;

    ws->target = (Vec2){
        .x =
            ((ws->house->rect.x + (ws->house->rect.w / 2)) - (ws->rect.w / 2)) +
            ws->rect.w,
        .y =
            ((ws->house->rect.y) - ws->rect.h) + 24 // - roof offset, this sucks
    };
}

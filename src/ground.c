#include "ground.h"
#include "main.h"

bool ground_new(GroundBlock **ground_b, SDL_Renderer *renderer) {
    *ground_b = calloc(1, sizeof(GroundBlock));
    if (*ground_b == NULL) {
        fprintf(stderr, "Error while allocating memory for (ground sprite)\n");
        return false;
    }

    GroundBlock *gb = *ground_b;
    gb->leaf = calloc(1, sizeof(GrassLeaf));

    gb->renderer = renderer;
    gb->leaf->renderer = renderer;

    gb->image = IMG_LoadTexture(gb->renderer, "./res/grass_ground.png");
    gb->leaf->image = IMG_LoadTexture(gb->renderer, "./res/grass_leaves.png");
    if (!gb->image) {
        fprintf(stderr, "Error while loading texture\n");
        return false;
    }

    if (!SDL_SetTextureScaleMode(gb->leaf->image, SDL_SCALEMODE_NEAREST)) {
        fprintf(stderr, "Error while setting scale mode (leaves)\n");
        return false;
    }

    if (!SDL_SetTextureScaleMode(gb->image, SDL_SCALEMODE_NEAREST)) {
        fprintf(stderr, "Error while setting scale mode.\n");
        return false;
    }

    float raw_tw, raw_th;
    if (!SDL_GetTextureSize(gb->image, &raw_tw, &raw_th)) {
        fprintf(stderr, "Error while getting rect size.\n");
        return false;
    }

    float raw_leaf_tw, raw_leaf_th;
    if (!SDL_GetTextureSize(gb->leaf->image, &raw_leaf_tw, &raw_leaf_th)) {
        fprintf(stderr, "Error while getting rect size.\n");
        return false;
    }

    int leaf_sprites = 3;

    gb->tile_w = raw_tw * PLAYER_SCALE_N;
    gb->tile_h = raw_th * PLAYER_SCALE_N;

    gb->leaf->tile_w = raw_leaf_tw * PLAYER_SCALE_N / leaf_sprites;
    gb->leaf->tile_h = raw_leaf_th * PLAYER_SCALE_N;


    // +2 for a margin an extra block on both sides
    gb->count = (int)SDL_ceilf(WINDOW_WIDTH / gb->tile_w) + 2;
    gb->rects = calloc(gb->count, sizeof(SDL_FRect));

    gb->leaf->count = gb->count;
    gb->leaf->rects = calloc(gb->leaf->count, sizeof(SDL_FRect));

    gb->leaf->sprite_numbers = calloc(gb->leaf->count, sizeof(int));
    gb->leaf->flipped_sprites = calloc(gb->leaf->count, sizeof(bool));

    if (!gb->leaf->sprite_numbers) {
        fprintf(stderr, "Mem allocation error (1)");
        return false;
    }

    if (!gb->leaf->flipped_sprites) {
        fprintf(stderr, "Mem allocation error (2)");
        return false;
    }

    if (!gb->rects) {
        fprintf(stderr, "Error while setting ground rect");
        return false;
    }

    if (!gb->leaf->rects) {
        fprintf(stderr, "Error while setting leaves rect");
        return false;
    }

    for (int i = 0; i < gb->count; i++) {
        gb->rects[i].w = gb->tile_w;
        gb->rects[i].h = gb->tile_h;

        gb->leaf->rects[i].w = gb->leaf->tile_w;
        gb->leaf->rects[i].h = gb->leaf->tile_h;
    
        gb->leaf->sprite_numbers[i] = rand() % leaf_sprites;
        gb->leaf->flipped_sprites[i] = rand() % 2;
    }

    return true;
}

void ground_free(GroundBlock **ground_b) {
    if (*ground_b) {
        GroundBlock *gb = *ground_b;
        if (gb->image) {
            SDL_DestroyTexture(gb->image);
            gb->image = NULL;
        }

        if (gb->leaf->image) {
            SDL_DestroyTexture(gb->leaf->image);
            gb->leaf->image = NULL;
        }

        if (gb->rects) {
            free(gb->rects);
            gb->rects = NULL;
        }

        if (gb->leaf->rects) {
            free(gb->leaf->rects);
            gb->leaf->rects = NULL;
        }

        if (gb->leaf->sprite_numbers) {
            free(gb->leaf->sprite_numbers);
            gb->leaf->sprite_numbers = NULL;
        }

        if (gb->leaf->flipped_sprites) {
            free(gb->leaf->flipped_sprites);
            gb->leaf->flipped_sprites = NULL;
        }

        gb->renderer = NULL;
        gb->leaf->renderer = NULL;
        free(gb->leaf);
        gb->leaf = NULL;
        free(gb);
        gb = NULL;
        *ground_b = NULL;
        printf("Ground sprite free.\n");
    }
}

void ground_update(GroundBlock *gb, Player *p) {
    for (int i = 0; i < gb->count; i++) {
        gb->rects[i].x = (i * gb->tile_w) - gb->tile_w;
        gb->rects[i].y = WINDOW_HEIGHT - gb->tile_h;

        gb->leaf->rects[i].x = gb->rects[i].x;
        gb->leaf->rects[i].y = gb->rects[i].y - gb->tile_h;
    }


    // has to compesate for nose size
    int nose_margin = 45;
    for (int i = 0; i < gb->count; i++) {
        if ((p->rect.y + p->rect.h) >= gb->rects[i].y &&
            (p->rect.x + p->rect.w) - nose_margin > (gb->rects[i].x) && 
            p->rect.x + nose_margin < (gb->rects[i].x + gb->tile_w)
        ) {
            p->rect.y = gb->rects[i].y - p->rect.h;
            p->vel.y = 0;
            p->is_jumping = false;
        }
    }


}

void ground_draw(const GroundBlock *gb) {
    SDL_FRect frame = {0, 0, 8, 8};
    SDL_FRect leaf_frame = {0, 0, 8, 8};

    bool flipped;
    for (int i = 0; i < gb->count; i++) {
        SDL_RenderTexture(gb->renderer, gb->image, &frame, &gb->rects[i]);

        leaf_frame.x = gb->leaf->sprite_numbers[i] * 8;
        flipped = gb->leaf->flipped_sprites[i];

        SDL_RenderTextureRotated(gb->leaf->renderer, gb->leaf->image, &leaf_frame, &gb->leaf->rects[i], 0, NULL, flipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    }
}   

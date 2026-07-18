#include "house.h"
#include "main.h"

bool house_new(House **house, SDL_Renderer *renderer) {
    *house = calloc(1, sizeof(House));
    if (*house == NULL) {
        fprintf(stderr, "Failed to allocate memory for 'House' struct");
        return false;
    }

    House *h = *house;
    h->renderer = renderer;

    SDL_Texture *house_sheet =
        IMG_LoadTexture(h->renderer, "./res/house_sprite_sheet.png");
    if (!house_sheet) {
        fprintf(stderr, "Error loading house sprite sheet");
        return false;
    }

    if (!SDL_SetTextureScaleMode(house_sheet, SDL_SCALEMODE_NEAREST)) {
        fprintf(stderr,
                "Error while setting scale mode, house sprite sheet: %s\n",
                SDL_GetError());
        return false;
    }
    h->house_width = 16, h->house_height = 16;
    // an empty texture 16x16 pixels big
    h->image = SDL_CreateTexture(h->renderer, SDL_PIXELFORMAT_RGBA32,
                                 SDL_TEXTUREACCESS_TARGET, h->house_width, h->house_height);

    if (!h->image) {
        fprintf(stderr, "Error while creating house texture: %s\n",
                SDL_GetError());

        SDL_DestroyTexture(house_sheet);
        return false;
    }

    SDL_SetTextureScaleMode(h->image, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureBlendMode(h->image, SDL_BLENDMODE_BLEND);

    // source rects
    SDL_FRect bottom_left = {0, 0, 8, 8};
    SDL_FRect bottom_right = {8, 0, 8, 8};
    SDL_FRect top_left = {16, 0, 8, 8};
    SDL_FRect top_right = {24, 0, 8, 8};

    // dest rects
    SDL_FRect top_left_dt = {0, 0, 8, 8};
    SDL_FRect top_right_dt = {8, 0, 8, 8};
    SDL_FRect bottom_left_dt = {0, 8, 8, 8};
    SDL_FRect bottom_right_dt = {8, 8, 8, 8};

    // save the previous render target for a moment
    SDL_Texture *prev_target = SDL_GetRenderTarget(h->renderer);
    SDL_SetRenderTarget(h->renderer, h->image);
    SDL_SetRenderDrawColor(h->renderer, 0, 0, 0, 0);

    // lowkey useless
    SDL_RenderClear(h->renderer);

    SDL_RenderTexture(h->renderer, house_sheet, &bottom_left, &bottom_left_dt);
    SDL_RenderTexture(h->renderer, house_sheet, &bottom_right, &bottom_right_dt);
    SDL_RenderTexture(h->renderer, house_sheet, &top_left, &top_left_dt);
    SDL_RenderTexture(h->renderer, house_sheet, &top_right, &top_right_dt);

    // return the render target to the screen;
    SDL_SetRenderTarget(h->renderer, prev_target);

    // dont need the spritesheet anymore
    SDL_DestroyTexture(house_sheet);


    h->rect.w = h->house_width * PLAYER_SCALE_N;
    h->rect.h = h->house_height * PLAYER_SCALE_N;

    return true;
}

void house_free(House **house) {
    if (*house) {
        House *h = *house;

        if (h->image) {
            SDL_DestroyTexture(h->image);
            h->image = NULL;
        }

        h->renderer = NULL;

        free(h);
        *house = NULL;
        printf("House sprite free.\n");
    }
}

void house_update(House *h, GroundBlock *ground, Player *p) {
    // put the house in the middle
    h->rect.x = (WINDOW_WIDTH / 2.0f) - (h->rect.w / 2.0f);
    h->rect.y = (WINDOW_HEIGHT - h->rect.h) - ground->tile_h;

    // this is bad
    int roof_sprite_margin = 2 * PLAYER_SCALE_N;

    SDL_FRect col_rect = {h->rect.x, h->rect.y + roof_sprite_margin, h->rect.w,
                          h->rect.h - roof_sprite_margin};

    bool p_collision_x = ((p->rect.x + p->rect.w) > col_rect.x) &&
                         (p->rect.x < col_rect.x + col_rect.w);
    bool p_collision_y = ((p->rect.y + p->rect.h) > col_rect.y) &&
                         (p->rect.y < col_rect.y + col_rect.h);

    // if the player would be inside the house
    if (p_collision_x && p_collision_y) {
        // how much the box overlaps in each dir
        float h_left_overlap = (p->rect.x + p->rect.w) - col_rect.x;
        float h_right_overlap = (col_rect.x + col_rect.w) - p->rect.x;

        float h_top_overlap = (p->rect.y + p->rect.h) - col_rect.y;
        float h_bottom_overlap = (col_rect.y + col_rect.h) - p->rect.y;

        float min_x = SDL_min(h_left_overlap, h_right_overlap);
        float min_y = SDL_min(h_top_overlap, h_bottom_overlap);

        // if the x overlap is shallower, side collision
        if (min_x < min_y) {
            if (h_left_overlap < h_right_overlap) {
                p->rect.x = col_rect.x - p->rect.w;
            } else {
                p->rect.x = col_rect.x + col_rect.w;
            }

            p->vel.x = 0;
        } 
        // collision on top (or bottom but like the house is on the ground so)
        else {
            if (h_top_overlap < h_bottom_overlap) {
                p->rect.y = col_rect.y - p->rect.h;
                p->is_jumping = false;
            } else {
                p->rect.y = col_rect.y + col_rect.h;
            }

            p->vel.y = 0;
        }
    }
}

void house_draw(const House *h) {
    SDL_FRect frame = {0, 0, 16, 16};
    SDL_RenderTexture(h->renderer, h->image, &frame, &h->rect);
}

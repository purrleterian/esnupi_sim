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

    SDL_Texture *house_sheet = IMG_LoadTexture(renderer, "./res/house_sprite_sheet.png");
    if (!house_sheet) {
        fprintf(stderr, "Error loading house sprite sheet");
        return false;
    }

    if (!SDL_SetTextureScaleMode(house_sheet, SDL_SCALEMODE_NEAREST)) {
    fprintf(stderr, "Error while setting scale mode, house sprite sheet");
        return false;
    }
    
    h->image = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 16, 16);

    if (!h->image) {
        fprintf(stderr, "Error while creating house texture");
        SDL_DestroyTexture(house_sheet);
        return false;
    }

    SDL_SetTextureScaleMode(h->image, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureBlendMode(h->image, SDL_BLENDMODE_BLEND);


    // source rects
    SDL_FRect bottom_left = {0, 0, 8, 8};
    SDL_FRect bottom_right = {8, 0, 8, 8};
    SDL_FRect top_left =    {16, 0, 8, 8};
    SDL_FRect top_right =   {24, 0, 8, 8};

    // dest rects
    
    SDL_FRect top_left_dt = {0, 0, 8, 8};
    SDL_FRect top_right_dt = {8, 0, 8, 8};
    SDL_FRect bottom_left_dt = {0, 8, 8, 8};
    SDL_FRect bottom_right_dt = {8, 8, 8, 8};

    
    // save the previous render target for a moment
    SDL_Texture *prev_target = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, h->image);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    SDL_RenderClear(renderer);

    SDL_RenderTexture(renderer, house_sheet, &bottom_left, &bottom_left_dt);
    SDL_RenderTexture(renderer, house_sheet, &bottom_right, &bottom_right_dt);
    SDL_RenderTexture(renderer, house_sheet, &top_left, &top_left_dt);
    SDL_RenderTexture(renderer, house_sheet, &top_right, &top_right_dt);


    // return the render target to the screen;
    SDL_SetRenderTarget(renderer, prev_target);
    
    // dont need the spritesheet anymore
    SDL_DestroyTexture(house_sheet);

    h->tile_w = 16 * PLAYER_SCALE_N;
    h->tile_h = 16 * PLAYER_SCALE_N;
    
    h->rect.w = h->tile_w;
    h->rect.h = h->tile_h;



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
        printf("House sprite free.");
    }
}


void house_update(House *h, GroundBlock*ground) {
    h->rect.x = (WINDOW_WIDTH / 2.0f) - (h->rect.w / 2.0f);
    h->rect.y = (WINDOW_HEIGHT - h->rect.h) - ground->tile_h;
}

void house_draw(const House *h) {
    SDL_FRect frame = {0, 0, 16, 16};
    SDL_RenderTexture(h->renderer, h->image, &frame, &h->rect);
}

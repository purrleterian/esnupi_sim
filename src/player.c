#include "player.h"



bool player_new(Player **player, SDL_Renderer *renderer) {
    *player = calloc(1, sizeof(Player));
    if (*player == NULL) {
        fprintf(stderr, "Error calloc of new player: %s\n", SDL_GetError());
        return false;
    }

    Player *p = *player;
    p->renderer = renderer;

    p->image = IMG_LoadTexture(p->renderer, "res/snoopy_spritesheet_v3.png");
    if (!p->image) {
        fprintf(stderr, "Error loading Texture: %s\n", SDL_GetError());
        return false;
    }

    if (!SDL_SetTextureScaleMode(p->image, SDL_SCALEMODE_NEAREST)) {
        fprintf(stderr, "Error setting scale mode: %s\n", SDL_GetError());
        return false;
    }

    if (!SDL_GetTextureSize(p->image, &p->rect.w, &p->rect.h)) {
        fprintf(stderr, "Error getting rect size: %s\n", SDL_GetError());
        return false;
    }

    float sprites_n = 5;


    p->rect.w *= PLAYER_SCALE_N / sprites_n; // width divided by the number of frames in the spritesheet
    p->rect.h *= PLAYER_SCALE_N;
    
    p->facing_right = true;

    p->keystate = SDL_GetKeyboardState(NULL);


    // p->rect.x = (WINDOW_WIDTH / 2.0f) - (p->rect.w / 2.0f);
    p->rect.x = 0;

    return true;
}

void player_free(Player **player) {
    if (*player) {
        Player *p = *player;
        if (p->image) {
            SDL_DestroyTexture(p->image);
            p->image = NULL;
        }

        p->renderer = NULL;
        p->keystate = NULL;

        free(p);
        p = NULL;
        *player = NULL;
        printf("Player Free.\n");
    }
}
void player_update(Player *p) {

    p->state = STANDING;
    p->accel.x = 0;
    p->accel.y = 0;


    if (p->keystate[SDL_SCANCODE_LEFT] || p->keystate[SDL_SCANCODE_A]) {
        p->accel.x = PLAYER_ACC * -1;
        // p->rect.x -= PLAYER_VEL;
        p->facing_right = false;
        p->state = WALKING;
    }

    if (p->keystate[SDL_SCANCODE_RIGHT] || p->keystate[SDL_SCANCODE_D]) {
        p->accel.x = PLAYER_ACC;
        // p->rect.x += PLAYER_VEL;

        p->facing_right = true;
        p->state = WALKING;
    }


    if (p->keystate[SDL_SCANCODE_SPACE] && !p->is_jumping) {
        p->vel.y = -18;
        p->is_jumping = true;
        p->state = JUMPING;
    }

    if (p->keystate[SDL_SCANCODE_DOWN] || p->keystate[SDL_SCANCODE_S]) {
        p->state = CROUCHING;
        p->accel.x /= 2;
    }

    if (p->keystate[SDL_SCANCODE_J]) {
        p->state = SITTING;
    }

    if (p->keystate[SDL_SCANCODE_K]) {
        p->state = SLEEPING;
    }

    p->accel.x += p->vel.x * -0.15;
    p->vel.x += p->accel.x;
    p->rect.x += p->vel.x + 0.5 * p->accel.x;

    p->vel.y += GRAVITY;
    p->rect.y += p->vel.y; 

    if (p->rect.y + p->rect.h > WINDOW_HEIGHT) {
        //p ->rect.y = WINDOW_HEIGHT - p->rect.h;
        p->vel.y = 0;
        p->is_jumping = false;
    }

    // horizontal boundaries
    if (p->rect.x > WINDOW_WIDTH) {
        p->rect.x = -1 * (p->rect.w);
    }

    else if (p->rect.x < (p->rect.w * - 1)) {
        p->rect.x = WINDOW_WIDTH; 
    }
}
void player_draw(const Player *p) {
    Uint32 ticks = SDL_GetTicks();

    Uint32 speed = 100;
    Uint32 seconds = ticks / speed;
    int walking_frames = 3;
    
    Uint32 sprite = 0;
    switch (p->state) {
        case WALKING: sprite = (seconds % walking_frames); break;
        case CROUCHING: sprite = 2; break;
        case STANDING: sprite = 0; break;
        case SITTING: sprite = 3; break;
        case SLEEPING: sprite = 4; break;

        default: break;

    }; 
    
    SDL_FRect frame = {sprite*8, 0, 8, 8};

    // create a rect for drawing so i can floor the positions and avoid
    // texture bleed :3
    SDL_FRect draw_rect = {
        SDL_floorf(p->rect.x),
        SDL_floorf(p->rect.y),
        p->rect.w,
        p->rect.h
    };

    SDL_RenderTextureRotated(p->renderer, p->image, &frame, &draw_rect,
                            0, NULL, 
                             !p->facing_right ? 
                             SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}


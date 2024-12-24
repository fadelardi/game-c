#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

typedef enum {
    AnimationDelayWalk = 5,
    AnimationDelayIdle = 10,
    AnimationDelayAttack = 7
} AnimationDelay;

typedef struct {
    int current_frame;
    int frame_count;
    int width;
    int height;
    int key_down_activation;
    int frame_update_delay;
    SDL_Texture *texture;
    SDL_FRect sprite_rect;
} Animation;

Animation *create_animation(int frame_count, const char* asset_tag, int key_down_activation, AnimationDelay frame_update_delay, int width, int height);
int update_animation(Animation *a);

#endif
#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

typedef struct {
    int current_frame;
    int frame_count;
    int width;
    int height;
    int key_down_activation;
    SDL_Texture *texture;
    SDL_FRect sprite_rect;
} Animation;

Animation *create_animation(int width, int height, int frame_count, int anination_id, int key_down_activation);
int update_animation(Animation *a);

#endif
#include "animation.h"
#include "assets.h"

Animation *create_animation(int width, int height, int frame_count, int anination_id, int key_down_activation) {
    Animation* a = (Animation*)SDL_malloc(sizeof(Animation));

    if (!a) {
        SDL_Log("Failed to allocate memory for animation");
        return NULL;
    }

    a->current_frame = 0;
    a->frame_count = frame_count;
    a->width = width;
    a->height = height;
    a->key_down_activation = key_down_activation;
    a->texture = assets[anination_id].texture;

    SDL_FRect sprite_rect = { 0, 0, width, height };

    a->sprite_rect = sprite_rect;

    return a;
}

int update_animation(Animation *a) {
    a->current_frame++;

    if (a->current_frame > a->frame_count - 1) {
        a->current_frame = 0;
    }

    a->sprite_rect.x = (a->width * a->current_frame);

    return !a->key_down_activation && a->current_frame == 0;
}
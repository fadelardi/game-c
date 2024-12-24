#include "animation.h"
#include "assets.h"
#include "base_entity.h"

Animation *create_animation(int frame_count, const char* asset_tag, int key_down_activation, AnimationDelay frame_update_delay, int width, int height) {
    Animation* a = (Animation*)SDL_malloc(sizeof(Animation));

    if (!a) {
        SDL_Log("Failed to allocate memory for animation");
        return NULL;
    }

    if (width == 0) {
        width = DEFAULT_CHAR_SIZE;
    }

    if (height == 0) {
        height = DEFAULT_CHAR_SIZE;
    }

    a->current_frame = 0;
    a->frame_count = frame_count;
    a->width = width;
    a->height = height;
    a->key_down_activation = key_down_activation;
    a->frame_update_delay = frame_update_delay;
    a->texture = get_asset_by_tag(asset_tag);

    SDL_FRect sprite_rect = { 0, 0, a->width, a->height };

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
#include "animation.h"
#include "assets.h"
#include "base_entity.h"

static const char* anim_asset_tag[] = {
    "witch_idle",
    "witch_walk",
    "witch_attack_1",
    "npc_idle",
    "npc_walk",
};

static SDL_FRect witch_attack_1_hitbox = { 60, 30, 50, 50 };

static AnimationConf anim_conf[] = {
    { 6, 1, AnimationDelayIdle, 0, 0, NULL },
    { 6, 1, AnimationDelayWalk, 0, 0, NULL },
    { 5, 0, AnimationDelayAttack, 0, 0, &witch_attack_1_hitbox },
    { 6, 1, AnimationDelayIdle, 0, 0, NULL },
    { 6,1, AnimationDelayWalk, 0, 0, NULL }
};

int get_conf_index(const char* asset_tag) {
    for (int i = 0; i < sizeof(anim_asset_tag) / sizeof(anim_asset_tag[0]); i++) {
        if (SDL_strcmp(asset_tag, anim_asset_tag[i]) == 0) {
            return i;
        }
    }

    return -1;
}

Animation *create_animation(const char* asset_tag) {
    int index = get_conf_index(asset_tag);

    if (index == -1) {
        SDL_Log("Failed to find animation configuration for asset tag: %s", asset_tag);
        return NULL;
    }

    AnimationConf conf = anim_conf[index];
    Animation* a = (Animation*)SDL_malloc(sizeof(Animation));

    if (!a) {
        SDL_Log("Failed to allocate memory for animation");
        return NULL;
    }

    int width = DEFAULT_CHAR_SIZE;
    int height = DEFAULT_CHAR_SIZE;

    if (conf.width != 0) {
        width = conf.width;
    }

    if (conf.height != 0) {
        height = conf.height;
    }

    a->current_frame = 0;
    a->frame_count = conf.frame_count;
    a->width = width;
    a->height = height;
    a->key_down_activation = conf.key_down_activation;
    a->frame_update_delay = conf.frame_update_delay;
    a->texture = get_asset_by_tag(asset_tag);

    SDL_FRect sprite_rect = { 0, 0, a->width, a->height };

    a->sprite_rect = sprite_rect;
    a->hitbox = NULL;
    a->hitbox_x = 0;
    a->hitbox_y = 0;
    
    if (conf.hitbox != NULL) {
        a->hitbox = (SDL_FRect*)SDL_malloc(sizeof(SDL_FRect));
        *a->hitbox = *conf.hitbox;
        a->hitbox_x = a->hitbox->x;
        a->hitbox_y = a->hitbox->y;
    }

    return a;
}

int update_animation(Animation *a) {
    a->current_frame++;

    if (a->current_frame > a->frame_count - 1) {
        a->current_frame = 0;
    }

    a->sprite_rect.x = (a->width * a->current_frame);

    if (a->hitbox != NULL && a->current_frame == 0) {
        a->hitbox->x = a->hitbox_x;
        a->hitbox->y = a->hitbox_y;
    }

    return !a->key_down_activation && a->current_frame == 0;
}

void free_animation(Animation *a) {
    if (a->hitbox != NULL) {
        SDL_free(a->hitbox);
    }
    SDL_free(a);
}
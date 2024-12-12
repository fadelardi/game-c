#include "constants.h"
#include "char_entity.h"

static const int FRAME_UPDATE_DELAY = 10;
static const int MOVE_SPEED = 5;
static const int CHAR_HEIGHT = 128;

Animation *create_animation(int width, int height, int frame_count, SDL_Texture *texture, int key_down_activation) {
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
    a->texture = texture;

    SDL_FRect sprite_rect = { 0, 0, width, height };

    a->sprite_rect = sprite_rect;

    return a;
}

CharEntity *create_char_entity(int x, int y, SDL_Texture *images[]) {
    CharEntity* e = (CharEntity*)SDL_malloc(sizeof(CharEntity));

    if (!e) {
        SDL_Log("Failed to allocate memory for char entity");
        return NULL;
    }

    e->x = x;
    e->y = y;
    e->direction = 1;
    e->anim_delay_counter = 0;
    SDL_FRect v = { 0, 0, CHAR_HEIGHT * 2, CHAR_HEIGHT * 2 };
    e->dest_rect = v;

    Animation* idle_animation = create_animation(CHAR_HEIGHT, CHAR_HEIGHT, 6, images[WITCH_IDLE_IDX], 1);
    Animation* walk_animation = create_animation(CHAR_HEIGHT, CHAR_HEIGHT, 6, images[WITCH_WALK_IDX], 1);
    Animation* quick_attack_animation = create_animation(CHAR_HEIGHT, CHAR_HEIGHT, 5, images[WITCH_Q_ATTACK_IDX], 0);

    e->walk_animation = walk_animation;
    e->idle_animation = idle_animation;
    e->quick_attack_animation = quick_attack_animation;
    e->current_animation = idle_animation;

    return e;
}

void char_move(CharEntity *c, SDL_Scancode dir) {
    c->current_animation = c->walk_animation;

    switch(dir) {
        case SDL_SCANCODE_LEFT:
            c->x -= MOVE_SPEED;
            c->direction = -1;
            break;
        case SDL_SCANCODE_RIGHT:
            c->x += MOVE_SPEED;
            c->direction = 1;
            break;
        case SDL_SCANCODE_UP:
            c->y -= MOVE_SPEED;
            break;
        case SDL_SCANCODE_DOWN:
            c->y += MOVE_SPEED;
            break;
        default:
            break;
    }
}

void char_attack(CharEntity *c) {
    c->current_animation = c->quick_attack_animation;
}

void reset_animation(CharEntity *c) {
    c->current_animation = c->idle_animation;
}

void update_animation(CharEntity *c) {
    Animation* a = c->current_animation;

    a->current_frame++;

    if (a->current_frame > a->frame_count - 1) {
        a->current_frame = 0;
        c->current_animation = c->idle_animation;
    }

    a->sprite_rect.x = (a->width * a->current_frame);
}

void update_character(CharEntity *c) {
    c->dest_rect.x = c->x;
    c->dest_rect.y = c->y;
    c->anim_delay_counter++;

    if (c->anim_delay_counter == FRAME_UPDATE_DELAY) {
        update_animation(c);
        c->anim_delay_counter = 1;
    }
}

void free_char_resources(CharEntity *c) {
    SDL_free(c->idle_animation);
    SDL_free(c->walk_animation);
    SDL_free(c->quick_attack_animation);
    SDL_free(c);
}



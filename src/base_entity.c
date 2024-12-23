#include <SDL3/SDL_main.h>
#include "base_entity.h"
#include "constants.h"

static const int FRAME_UPDATE_DELAY = 10;
static const int MOVE_SPEED = 5;

BaseEntity *init_base_entity(int x, int y, Animation *idle_animation, Animation *walk_animation) {
    BaseEntity *e = (BaseEntity*) SDL_malloc(sizeof(BaseEntity));

    if (e == NULL) {
        SDL_Log("Failed to allocate memory for base entity: %s", SDL_GetError());
        SDL_Quit();
        return NULL;
    }

    e->x = x;
    e->y = y;
    e->direction = 1;
    e->move_speed = MOVE_SPEED;
    e->anim_delay_counter = 0;
    e->current_animation = idle_animation;
    e->idle_animation = idle_animation;
    e->walk_animation = walk_animation;
    SDL_FRect visual_rect = { x, y, DEFAULT_CHAR_SIZE * 2, DEFAULT_CHAR_SIZE * 2 };
    e->visual_rect = visual_rect;
    e->hitbox = visual_rect;

    return e;
}

void move_entity(BaseEntity *e, int dir) {
    enum DIR {
        D_LEFT,
        D_RIGHT,
        D_UP,
        D_DOWN
    };
    e->current_animation = e->walk_animation;

    switch(dir) {
        case D_LEFT:
            e->x -= e->move_speed;
            e->direction = -1;
            break;
        case D_RIGHT:
            e->x += e->move_speed;
            e->direction = 1;
            break;
        case D_UP:
            e->y -= e->move_speed;
            break;
        case D_DOWN:
            e->y += e->move_speed;
            break;
        default:
            break;
    }
}

void render_entity(BaseEntity *e, SDL_Renderer *renderer) {
    if (e->direction == -1) {
        SDL_RenderTextureRotated(renderer, e->current_animation->texture, &e->current_animation->sprite_rect, &e->visual_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
    } else {
        SDL_RenderTexture(renderer, e->current_animation->texture, &e->current_animation->sprite_rect, &e->visual_rect);
    }
}

void update_entity(BaseEntity *e) {
    e->visual_rect.x = e->x;
    e->visual_rect.y = e->y;
    e->anim_delay_counter++;

    if (e->anim_delay_counter == FRAME_UPDATE_DELAY) {
        int stop_animation = update_animation(e->current_animation);
        if (stop_animation) {
            reset_animation(e);
        }
        e->anim_delay_counter = 1;
    }
}

void reset_animation(BaseEntity *e) {
    e->current_animation = e->idle_animation;
}

void free_entity(BaseEntity *e) {
    SDL_free(e->idle_animation);
    SDL_free(e->walk_animation);
    SDL_free(e);
}

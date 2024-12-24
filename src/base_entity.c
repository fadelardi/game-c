#include <SDL3/SDL_main.h>
#include "base_entity.h"

static const int MOVE_SPEED = 5;
static const int X_OFFSET = 48 * 2;
static const int Y_OFFSET = DEFAULT_CHAR_SIZE;

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
    SDL_FRect hitbox = { x + X_OFFSET, y + Y_OFFSET, DEFAULT_CHAR_SIZE / 2, DEFAULT_CHAR_SIZE };
    e->visual_rect = visual_rect;
    e->hitbox = hitbox;

    return e;
}

void move_entity(BaseEntity *e, MOVEMENT_DIRECTION dir) {
    e->current_animation = e->walk_animation;

    switch(dir) {
        case D_LEFT:
            e->x -= e->move_speed;
            e->direction = -1;
            e->hitbox.x = e->x + X_OFFSET;
            break;
        case D_RIGHT:
            e->x += e->move_speed;
            e->direction = 1;
            e->hitbox.x = e->x + X_OFFSET;
            break;
        case D_UP:
            e->y -= e->move_speed;
            e->hitbox.y = e->y + Y_OFFSET;
            break;
        case D_DOWN:
            e->y += e->move_speed;
            e->hitbox.y = e->y + Y_OFFSET;
            break;
        default:
            break;
    }
}

bool check_collision(BaseEntity *e1, BaseEntity *e2) {
    return SDL_HasRectIntersectionFloat(&e1->hitbox, &e2->hitbox);
}

void render_entity(BaseEntity *e, SDL_Renderer *renderer) {
    if (e->direction == -1) {
        SDL_RenderTextureRotated(renderer, e->current_animation->texture, &e->current_animation->sprite_rect, &e->visual_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
    } else {
        SDL_RenderTexture(renderer, e->current_animation->texture, &e->current_animation->sprite_rect, &e->visual_rect);
        // SDL_Log("Rendering entity %f %f %f %f", e->hitbox.x, e->hitbox.y, e->hitbox.w, e->hitbox.h); 
        SDL_SetRenderDrawColor(renderer, 0xBB, 0xB1, 0xCC, SDL_ALPHA_OPAQUE);
    }
    SDL_RenderRect(renderer, &e->hitbox);
}

void update_entity(BaseEntity *e) {
    e->visual_rect.x = e->x;
    e->visual_rect.y = e->y;
    e->anim_delay_counter++;

    if (e->anim_delay_counter >= e->current_animation->frame_update_delay) {
        int stop_animation = update_animation(e->current_animation);
        if (stop_animation) {
            reset_animation(e);
        }
        e->anim_delay_counter = 0;
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

#ifndef BASE_ENTITY_H
#define BASE_ENTITY_H

#include "animation.h"
#include "stats.h"
#include <stdbool.h>

#define DEFAULT_CHAR_SIZE 128

typedef struct {
    int x;
    int y;
    int direction;
    int anim_delay_counter;
    int move_speed;
    SDL_FRect visual_rect;
    SDL_FRect hitbox;
    Stats stats;
    Animation *current_animation;
    Animation *idle_animation;
    Animation *walk_animation;
    Animation *dying_animation;
} BaseEntity;

typedef enum {
    D_LEFT,
    D_RIGHT,
    D_UP,
    D_DOWN
} MovementDirection;

BaseEntity *init_base_entity(int x, int y, Animation *idle_animation, Animation *walk_animation, Animation *dying_animation);
void render_entity(BaseEntity *e, SDL_Renderer *renderer);
void update_entity(BaseEntity *e);
bool check_collision(BaseEntity *e1, BaseEntity *e2);
bool check_hitbox_collision(SDL_FRect *hitbox, BaseEntity *e);
void move_entity(BaseEntity *e, MovementDirection dir);
void free_entity(BaseEntity *e);
void reset_animation(BaseEntity *e);

#endif
#ifndef BASE_ENTITY_H
#define BASE_ENTITY_H

#include "animation.h"

typedef struct {
    int x;
    int y;
    int direction;
    int anim_delay_counter;
    int move_speed;
    SDL_FRect dest_rect;
    Animation *current_animation;
    Animation *idle_animation;
    Animation *walk_animation;
} BaseEntity;

BaseEntity *init_base_entity(int x, int y, Animation *idle_animation, Animation *walk_animation);
void render_entity(BaseEntity *e, SDL_Renderer *renderer);
void update_entity(BaseEntity *e);
void move_entity(BaseEntity *e, int dir);
void free_entity(BaseEntity *e);
void reset_animation(BaseEntity *e);

#endif
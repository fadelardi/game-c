#ifndef CHAR_ENTITY_H
#define CHAR_ENTITY_H

#include <SDL3/SDL.h>
#include "base_entity.h"

typedef struct {
    BaseEntity *base;
    Animation *quick_attack_animation;
} PlayerEntity;

PlayerEntity* create_char_entity(int x, int y);
void char_attack(PlayerEntity *c);
bool is_hitting_entity(PlayerEntity *c, BaseEntity *e);
void render_player_char(SDL_Renderer *renderer, PlayerEntity *player_entity);
void free_player_entity(PlayerEntity *c);

#endif // CHAR_ENTITY_H

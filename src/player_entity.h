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
void free_main_char(PlayerEntity *c);

#endif // CHAR_ENTITY_H

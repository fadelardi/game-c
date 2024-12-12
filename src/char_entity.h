#ifndef CHAR_ENTITY_H
#define CHAR_ENTITY_H

#include <SDL3/SDL.h>
#include "map.h"

typedef struct {
    int current_frame;
    int frame_count;
    int width;
    int height;
    SDL_Texture *texture;
    SDL_FRect tile_rect;
} Animation;

typedef struct {
    int x;
    int y;
    int anim_delay_counter;
    SDL_FRect dest_rect;
    Animation *idle_animation;
    Animation *walk_animation;
    Animation *current_animation;
} CharEntity;

void update_character(CharEntity *c, TileMap *t);
void free_char_resources(CharEntity *c);
void move_char(CharEntity *c, SDL_Scancode);
void reset_animation(CharEntity *c);

CharEntity* create_char_entity(int x, int y, SDL_Texture *images[]);

#endif // CHAR_ENTITY_H

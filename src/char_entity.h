#ifndef CHAR_ENTITY_H
#define CHAR_ENTITY_H

#include <SDL3/SDL.h>

typedef struct {
    int current_frame;
    int frame_count;
    int width;
    int height;
    int key_down_activation;
    SDL_Texture *texture;
    SDL_FRect sprite_rect;
} Animation;

typedef struct {
    int x;
    int y;
    int direction;
    int anim_delay_counter;
    SDL_FRect dest_rect;
    Animation *idle_animation;
    Animation *walk_animation;
    Animation *quick_attack_animation;
    Animation *current_animation;
} CharEntity;

void update_character(CharEntity *c);
void render_character(CharEntity *c,  SDL_Renderer *renderer);
void free_char_resources(CharEntity *c);
void char_move(CharEntity *c, SDL_Scancode);
void char_attack(CharEntity *c);
void reset_animation(CharEntity *c);

CharEntity* create_char_entity(int x, int y);

#endif // CHAR_ENTITY_H

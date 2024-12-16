#include "char_entity.h"
#include "constants.h"

MainChar *create_char_entity(int x, int y) {
  MainChar *e = (MainChar *)SDL_malloc(sizeof(MainChar));

  if (!e) {
    SDL_Log("Failed to allocate memory for char entity");
    return NULL;
  }

  Animation *idle_animation = create_animation(
      DEFAULT_CHAR_SIZE, DEFAULT_CHAR_SIZE, 6, WITCH_IDLE_IDX, 1);
  Animation *walk_animation = create_animation(
      DEFAULT_CHAR_SIZE, DEFAULT_CHAR_SIZE, 6, WITCH_WALK_IDX, 1);

  e->base = init_base_entity(x, y, idle_animation, walk_animation);

  Animation *quick_attack_animation = create_animation(
      DEFAULT_CHAR_SIZE, DEFAULT_CHAR_SIZE, 5, WITCH_Q_ATTACK_IDX, 0);

  e->quick_attack_animation = quick_attack_animation;

  return e;
}

void char_attack(MainChar *c) {
  c->base->current_animation = c->quick_attack_animation;
}

void free_main_char(MainChar *c) {
  free_entity(c->base);
  SDL_free(c->quick_attack_animation);
  SDL_free(c);
}

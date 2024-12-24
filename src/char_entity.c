#include "char_entity.h"
#include "animation.h"

MainChar *create_char_entity(int x, int y) {
  MainChar *e = (MainChar *)SDL_malloc(sizeof(MainChar));

  if (!e) {
    SDL_Log("Failed to allocate memory for char entity");
    return NULL;
  }

  Animation *idle_animation =
      create_animation(6, "witch_idle", 1, AnimationDelayIdle, 0, 0);
  Animation *walk_animation =
      create_animation(6, "witch_walk", 1, AnimationDelayWalk, 0, 0);

  e->base = init_base_entity(x, y, idle_animation, walk_animation);

  Animation *quick_attack_animation =
      create_animation(5, "witch_attack_1", 0, AnimationDelayAttack, 0, 0);

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

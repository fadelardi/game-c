#include "player_entity.h"
#include "animation.h"
#include "base_entity.h"

PlayerEntity *create_char_entity(int x, int y) {
  PlayerEntity *e = (PlayerEntity *)SDL_malloc(sizeof(PlayerEntity));

  if (!e) {
    SDL_Log("Failed to allocate memory for char entity");
    return NULL;
  }

  Animation *idle_animation = create_animation("witch_idle");
  Animation *walk_animation = create_animation("witch_walk");
  Animation *dying_animation = create_animation("witch_death");

  e->base = init_base_entity(x, y, idle_animation, walk_animation, dying_animation);

  Animation *quick_attack_animation = create_animation("witch_attack_1");

  e->quick_attack_animation = quick_attack_animation;

  return e;
}

void char_attack(PlayerEntity *c) {
  if (c->base->current_animation == c->quick_attack_animation) {
    return;
  }
  c->base->current_animation = c->quick_attack_animation;

  SDL_FRect *attack_hitbox = c->quick_attack_animation->hitbox;

  if (attack_hitbox != NULL) {
    attack_hitbox->x += c->base->hitbox.x - (c->base->direction == 1 ? 0 : c->quick_attack_animation->init_hitbox_x + attack_hitbox->w);
    attack_hitbox->y += c->base->hitbox.y;
  }
}

bool is_hitting_entity(PlayerEntity *c, BaseEntity *e) {
  Animation *cur_anim = c->base->current_animation;
  if (cur_anim == NULL) {
    return false;
  }

  if (cur_anim->hitbox != NULL && cur_anim->current_frame == cur_anim->frame_on_hit && c->base->anim_delay_counter == 0) {
    return check_hitbox_collision(cur_anim->hitbox, e);
  }

  return false;
}

void free_player_entity(PlayerEntity *c) {
  free_entity(c->base);
  free_animation(c->quick_attack_animation);
  SDL_free(c);
}

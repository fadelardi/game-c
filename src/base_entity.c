#include "base_entity.h"
#include "config.h"
#include <SDL3/SDL_log.h>

static const int MOVE_SPEED = 5;
static const int X_OFFSET = 48 * 2;
static const int Y_OFFSET = DEFAULT_CHAR_SIZE;

BaseEntity *init_base_entity(int x, int y, Animation *idle_animation,
                             Animation *walk_animation,
                             Animation *dying_animation) {
  BaseEntity *e = (BaseEntity *)SDL_malloc(sizeof(BaseEntity));

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
  e->dying_animation = dying_animation;
  SDL_FRect visual_rect = {x, y, DEFAULT_CHAR_SIZE * 2, DEFAULT_CHAR_SIZE * 2};
  SDL_FRect hitbox = {x + X_OFFSET, y + Y_OFFSET, DEFAULT_CHAR_SIZE / 2,
                      DEFAULT_CHAR_SIZE};
  e->visual_rect = visual_rect;
  e->hitbox = hitbox;
  e->stats = init_stats();

  return e;
}

void move_entity(BaseEntity *e, MovementDirection dir) {
  if (!e) {
    SDL_Log("Invalid movement direction: %d", dir);
    return;
  }

  if (e->current_animation != e->walk_animation) {
    e->current_animation = e->walk_animation;
  }

  int new_x = e->x;
  int new_y = e->y;
  const int ZOOMED_Y_OFFSET = Y_OFFSET * 2;

  switch (dir) {
  case D_LEFT:
    new_x -= e->move_speed;
    e->direction = -1;
    break;
  case D_RIGHT:
    new_x += e->move_speed;
    e->direction = 1;
    break;
  case D_UP:
    if (new_y > GROUND_TOP_Y - ZOOMED_Y_OFFSET) {
      new_y -= e->move_speed;
    }
    break;
  case D_DOWN:
    if (new_y < WINDOW_HEIGHT - GROUND_BOTTOM_Y) {
      new_y += e->move_speed;
    }
    break;
  default:
    return;
  }

  if (new_y < GROUND_TOP_Y - ZOOMED_Y_OFFSET) {
    new_y = GROUND_TOP_Y - ZOOMED_Y_OFFSET;
  } else if (new_y + ZOOMED_Y_OFFSET > WINDOW_HEIGHT - GROUND_BOTTOM_Y) {
    new_y = WINDOW_HEIGHT - GROUND_BOTTOM_Y - ZOOMED_Y_OFFSET;
  }

  if (e->x != new_x || e->y != new_y) {
    e->x = new_x;
    e->y = new_y;
    e->hitbox.x = e->x + X_OFFSET;
    e->hitbox.y = e->y + Y_OFFSET;
  }
}

bool check_collision(BaseEntity *e1, BaseEntity *e2) {
  return SDL_HasRectIntersectionFloat(&e1->hitbox, &e2->hitbox);
}

bool check_hitbox_collision(SDL_FRect *hitbox, BaseEntity *e) {
  return SDL_HasRectIntersectionFloat(hitbox, &e->hitbox);
}

void render_hitboxes(BaseEntity *e, SDL_Renderer *renderer) {
  if (e->current_animation->hitbox != NULL &&
      e->current_animation->current_frame > 0) {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xBB, SDL_ALPHA_OPAQUE);
    SDL_RenderRect(renderer, &e->hitbox);
    SDL_SetRenderDrawColor(renderer, 0xBB, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderRect(renderer, e->current_animation->hitbox);
  }
}

void render_entity(BaseEntity *e, SDL_Renderer *renderer) {
  if (e->direction == -1) {
    SDL_RenderTextureRotated(renderer, e->current_animation->texture,
                             &e->current_animation->sprite_rect,
                             &e->visual_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
  } else {
    SDL_RenderTexture(renderer, e->current_animation->texture,
                      &e->current_animation->sprite_rect, &e->visual_rect);
  }

  render_hitboxes(e, renderer);
}

void update_entity(BaseEntity *e) {
  if (e->stats.hp > -1) {
    e->visual_rect.x = e->x;
    e->visual_rect.y = e->y;
    e->anim_delay_counter++;

    if (e->anim_delay_counter >= e->current_animation->frame_update_delay) {
      bool stop_animation = update_animation(e->current_animation);
      if (stop_animation) {
        reset_animation(e);
      }
      e->anim_delay_counter = 0;
      if (e->current_animation == e->dying_animation &&
          e->current_animation->current_frame ==
              e->current_animation->frame_count - 1) {
        e->stats.hp = -1;
      }
    }
  }
}

void reset_animation(BaseEntity *e) {
  e->current_animation = e->idle_animation;
}

void free_entity(BaseEntity *e) {
  free_animation(e->idle_animation);
  free_animation(e->walk_animation);
  SDL_free(e);
}

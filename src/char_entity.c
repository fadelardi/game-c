
#include "constants.h"
#include "char_entity.h"

static const int FRAME_UPDATE_DELAY = 10;
static const int MOVE_SPEED = 5;

Animation *create_animation(int width, int height, int frame_count, SDL_Texture *texture) {
    Animation* a = (Animation*)SDL_malloc(sizeof(Animation));

    if (!a) {
        SDL_Log("Failed to allocate memory for animation"); 
        return NULL;
    } 

    a->current_frame = 0;
    a->frame_count = frame_count;
    a->width = width;
    a->height = height;
    a->texture = texture;

    SDL_FRect tile_rect = { 0, 0, width, height }; 

    a->tile_rect = tile_rect;
    
    return a;
}

CharEntity *create_char_entity(int x, int y, SDL_Texture *images[]) {
    const int HEIGHT = 128;
    CharEntity* e = (CharEntity*)SDL_malloc(sizeof(CharEntity));

    if (!e) {
        SDL_Log("Failed to allocate memory for char entity"); 
        return NULL;
    } 
    
    e->x = x;
    e->y = y;
    e-> anim_delay_counter = 0;
    SDL_FRect v = { 0, 0, HEIGHT * 2, HEIGHT * 2 };
    e->dest_rect = v;

    Animation* idle_animation = create_animation(HEIGHT, HEIGHT, 6, images[WITCH_IDLE_IDX]);
    Animation* walk_animation = create_animation(HEIGHT, HEIGHT, 6, images[WITCH_WALK_IDX]);

    e->walk_animation = walk_animation;
    e->idle_animation = idle_animation;
    e->current_animation = idle_animation;

    return e;
}

void move_char(CharEntity *c, SDL_Scancode dir) {
    c->current_animation = c->walk_animation;

    if (dir == SDL_SCANCODE_LEFT) {
        c->x -= MOVE_SPEED;
    } else if (dir == SDL_SCANCODE_RIGHT) {
        c->x += MOVE_SPEED;
    }
}

void reset_animation(CharEntity *c) {
    c->current_animation = c->idle_animation;
}

void update_animation(CharEntity *c) {
    Animation* a = c->current_animation;

    a->current_frame++;

    if (a->current_frame > a->frame_count - 1) {
        a->current_frame = 0;
    }

    a->tile_rect.x = (a->width * a->current_frame);
}

bool check_collision(SDL_FRect *a, SDL_FRect *b) {
    SDL_Rect a_rect = { (int)a->x, (int)a->y, (int)a->w, (int)a->h };
    SDL_Rect b_rect = { (int)b->x, (int)b->y, (int)b->w, (int)b->h };
    return SDL_HasRectIntersection(&a_rect, &b_rect);
}

void update_character(CharEntity *c, TileMap *map) {
    c->dest_rect.x = c->x;
    c->dest_rect.y += 5;

    for (int i = 0; i < map->tile_count; i++) {
        if (check_collision(&c->dest_rect, &map->tiles[i].dest_rect)) {
            c->dest_rect.y -= 5;
            break;
        }
    }

    c->anim_delay_counter++;
    if (c->anim_delay_counter == FRAME_UPDATE_DELAY) {
        update_animation(c);
        c->anim_delay_counter = 1;
    }
}

void free_char_resources(CharEntity *c) {
    SDL_free(c->idle_animation);
    SDL_free(c->walk_animation);
    SDL_free(c);
}


    
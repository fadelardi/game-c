#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include "char_entity.h"
#include "constants.h"
#include "utils.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *images[3];
static CharEntity *main_char;
// static TileMap map;

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Test", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    images[WITCH_IDLE_IDX] = load_sprite(renderer, "idle.png");
    images[WITCH_WALK_IDX] = load_sprite(renderer, "walk.png");
    images[WITCH_Q_ATTACK_IDX] = load_sprite(renderer, "attack_1.png");
    
    main_char = create_char_entity(0, 0, images);
    // map = create_map(images, WINDOW_HEIGHT);
    
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    } else if (event->type == SDL_EVENT_KEY_DOWN) {
        switch(event->key.scancode) {
            case SDL_SCANCODE_A:
                char_move(main_char, SDL_SCANCODE_LEFT);
                break;
            case SDL_SCANCODE_D:
                char_move(main_char, SDL_SCANCODE_RIGHT);
                break;
            case SDL_SCANCODE_W:
                char_move(main_char, SDL_SCANCODE_UP);
                break;
            case SDL_SCANCODE_S:
                char_move(main_char, SDL_SCANCODE_DOWN);
                break;
            case SDL_SCANCODE_Q:
                char_attack(main_char);
                break;
            default:
                break;
        }
    } else if (event->type == SDL_EVENT_KEY_UP) {
        if (main_char->current_animation->key_down_activation) {
            reset_animation(main_char);
        }
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    update_character(main_char);

    if (main_char->direction == -1) {
        SDL_RenderTextureRotated(renderer, main_char->current_animation->texture, &main_char->current_animation->sprite_rect, &main_char->dest_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
    } else {
        SDL_RenderTexture(renderer, main_char->current_animation->texture, &main_char->current_animation->sprite_rect, &main_char->dest_rect);
    }

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    free_char_resources(main_char);
    // free_map(&map);
}

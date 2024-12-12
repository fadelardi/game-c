#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include "char_entity.h"
#include "map.h"
#include "constants.h"
#include "utils.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *images[3];
static CharEntity *main_char;
static TileMap map;

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 500

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
    images[TILESET_IDX] = load_sprite(renderer, "map.png");
    
    main_char = create_char_entity(0, 0, images);
    map = create_map(images, WINDOW_HEIGHT);
    
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    } else if (event->type == SDL_EVENT_KEY_DOWN) {
        switch(event->key.scancode) {
            case SDL_SCANCODE_LEFT:
                move_char(main_char, SDL_SCANCODE_LEFT);
                break;
            case SDL_SCANCODE_RIGHT:
                move_char(main_char, SDL_SCANCODE_RIGHT);
                break;
            default:
                break;
        }
    } else if (event->type == SDL_EVENT_KEY_UP) {
        reset_animation(main_char);
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    update_character(main_char, &map);

    for (int i = 0; i < map.tile_count; i++) {
        SDL_RenderTexture(renderer, map.tiles[i].texture, &map.tiles[i].tile_rect, &map.tiles[i].dest_rect);
    }

    SDL_RenderTexture(renderer, main_char->current_animation->texture, &main_char->current_animation->tile_rect, &main_char->dest_rect);

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    free_char_resources(main_char);
    free_map(&map);
}

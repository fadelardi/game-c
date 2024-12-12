#include "assets.h"
#include <SDL3_image/SDL_image.h>

Asset assets[NUM_ASSETS] = {
    { NULL, "idle.png" },
    { NULL, "walk.png" },
    { NULL, "attack_1.png" }
};

SDL_Texture* load_sprite(SDL_Renderer *renderer, const char *sprite_name) {
    SDL_Texture *texture = NULL;
    SDL_Surface *surface = NULL;
    char *img_path = NULL;

    SDL_asprintf(&img_path, "%simages/%s", SDL_GetBasePath(), sprite_name);
    
    if (!img_path) { 
        SDL_Log("Failed to allocate memory for image path"); 
        return NULL;
    }
    surface = IMG_Load(img_path);

    SDL_free(img_path);

    if (!surface) {
        SDL_Log("Couldn't load bitmap: %s", SDL_GetError());

        return texture;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        return texture;
    }

    SDL_DestroySurface(surface);

    return texture;
}

void init_assets(SDL_Renderer* renderer) {
    for (int i = 0; i < NUM_ASSETS; i++) {
        assets[i].texture = load_sprite(renderer, assets[i].filepath);
        if (!assets[i].texture) {
            SDL_Log("Failed to load texture %s: %s", assets[i].filepath, SDL_GetError());
        }
    }
}

void free_assets() {
    for (int i = 0; i < NUM_ASSETS; i++) {
        if (assets[i].texture) {
            SDL_DestroyTexture(assets[i].texture);
            assets[i].texture = NULL;
        }
    }
}

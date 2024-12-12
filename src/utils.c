#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

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
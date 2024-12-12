#ifndef ASSETS_H
#define ASSETS_H

#include <SDL3/SDL.h>

#define NUM_ASSETS 3

typedef struct {
    SDL_Texture* texture;
    const char* filepath;
} Asset;

extern Asset assets[NUM_ASSETS];

void init_assets(SDL_Renderer* renderer);
void free_assets();

#endif // ASSETS_H

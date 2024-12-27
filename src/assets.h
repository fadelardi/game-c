#ifndef ASSETS_H
#define ASSETS_H

#include <SDL3/SDL.h>
#define NUM_ASSETS 8

typedef struct {
  SDL_Texture *texture;
  const char *filepath;
  char *tag;
} Asset;

extern Asset assets[NUM_ASSETS];

void init_assets(SDL_Renderer *renderer);
SDL_Texture *get_asset_by_tag(const char *tag);
void free_assets();

#endif // ASSETS_H

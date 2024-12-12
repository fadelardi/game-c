#ifndef MAP_H
#define MAP_H

#include <SDL3/SDL.h>

enum TileType {
    GROUND
};

typedef struct Tile {
    enum TileType tile_type;
    bool passable;
    bool visible;
    SDL_Texture *texture;
    SDL_FRect tile_rect;
    SDL_FRect dest_rect;
    int width;
    int height;
} Tile;

typedef struct TileMap {
    int tile_count;
    Tile *tiles;
} TileMap;

TileMap create_map(SDL_Texture *images[], int window_height);
void free_map(TileMap *tile_map);
#endif
#include "map.h"
#include "constants.h"

Tile *create_tile(int x, int y, SDL_Texture *texture, enum TileType tile_type) {
    const int WIDTH = 32;
    const int HEIGHT = 32;
    Tile* t = (Tile*) SDL_malloc(sizeof(Tile));

    SDL_FRect tile_rect = { 0, 64, WIDTH, HEIGHT };
    SDL_FRect dest_rect = { x, y, WIDTH * 2, HEIGHT * 2 };

    t->height = WIDTH;
    t->width = HEIGHT;
    t->passable = false;
    t->visible = true;
    t->texture = texture;
    t->tile_type = tile_type;
    t->tile_rect = tile_rect;
    t->dest_rect = dest_rect; 

    return t;
}

TileMap create_map(SDL_Texture *images[], const int WINDOW_HEIGHT) {
    const int NUM_TILES = 10;
    TileMap tile_map;
    tile_map.tile_count = NUM_TILES;
    Tile* map_layout = (Tile*) SDL_malloc(NUM_TILES * sizeof(Tile));
     if (!map_layout) {
        SDL_Log("Failed to allocate memory for map layout");
        return (TileMap){0};
    }
 
    for (int tile_idx = 0; tile_idx < NUM_TILES; tile_idx++) {
        // map_layout[tile_idx] = *create_tile(tile_idx * 64, WINDOW_HEIGHT - 64, images[TILESET_IDX], GROUND);
    }

    tile_map.tiles = map_layout;

    return tile_map;
}

void free_map(TileMap *tile_map) {
    SDL_free(tile_map->tiles);
}
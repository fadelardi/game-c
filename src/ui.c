#include <SDL3_ttf/SDL_ttf.h>

static TTF_TextEngine *engine = NULL;
static SDL_Renderer *ui_renderer = NULL;
static TTF_Font *font = NULL;
static SDL_Surface *text_surface = NULL;
static SDL_Texture *text_texture = NULL;
static const char *DEFAULT_FONT = "NotoSansMono-Regular.ttf";
static const int DIALOG_WIDTH = 400;
static const int DIALOG_HEIGHT = 100;

void init_ui(SDL_Renderer *renderer) {
  if (!TTF_Init()) {
    SDL_Log("Could not initialise TTF: %s", SDL_GetError());
    SDL_Quit();
  }

  ui_renderer = renderer;
  engine = TTF_CreateRendererTextEngine(ui_renderer);
  font = TTF_OpenFont(DEFAULT_FONT, 15.0f);

  if (font == NULL) {
    SDL_Log("Could not init font: %s", SDL_GetError());
    SDL_Quit();
  }

  if (engine == NULL) {
    SDL_Log("Could not init font engine %s", SDL_GetError());
    SDL_Quit();
  }
}

void render_ui() {
  int x = 20;
  int y = 20;

  SDL_SetRenderDrawColor(ui_renderer, 0xB4, 0xB4, 0xB9, SDL_ALPHA_OPAQUE);
  SDL_FRect rect = {x, y, DIALOG_WIDTH, DIALOG_HEIGHT};
  SDL_RenderFillRect(ui_renderer, &rect);

  SDL_SetRenderDrawColor(ui_renderer, 0xFF, 0x01, 0xB0, SDL_ALPHA_OPAQUE);
  SDL_Color black = {0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE};
  text_surface =
      TTF_RenderText_Solid(font, "This is an example dialogue.", 0, black);
  text_texture = SDL_CreateTextureFromSurface(ui_renderer, text_surface);

  if (text_surface == NULL) {
    SDL_Log("Failed to create text texture: %s", SDL_GetError());
    SDL_Quit();
  }
  SDL_FRect text_rect = {x + 5, y + 5, text_surface->w, text_surface->h};

  SDL_RenderTexture(ui_renderer, text_texture, NULL, &text_rect);
}

void free_ui() {
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_DestroyTexture(text_texture);
}

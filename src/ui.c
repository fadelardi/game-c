#include "dialogue.h"
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
    TTF_Quit();
    return;
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

void render_text_paragraph(char *text, int x, int y, int active) {
  SDL_Color text_colour = {0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE};
  if (active) {
    text_colour.r = 0xEE;
    text_colour.g = 0xEE;
    text_colour.b = 0xEE;
  }

  text_surface = TTF_RenderText_Solid_Wrapped(font, text, 0, text_colour, DIALOG_WIDTH);

  if (text_surface == NULL) {
    SDL_Log("Failed to create text surface: %s", SDL_GetError());
    SDL_Quit();
    return;
  }

  text_texture = SDL_CreateTextureFromSurface(ui_renderer, text_surface);

  if (text_texture == NULL) {
    SDL_Log("Failed to create text texture: %s", SDL_GetError());
    SDL_DestroySurface(text_surface);
    SDL_Quit();
    return;
  }

  SDL_FRect text_rect = {x, y, text_surface->w, text_surface->h};
  SDL_RenderTexture(ui_renderer, text_texture, NULL, &text_rect);
}

void render_dialogue(Dialogue *d) {
  const int LINE_HEIGHT = 5;
  if (d != NULL) {
    int x = 20;
    int y = 20;

    SDL_SetRenderDrawColor(ui_renderer, 0xB4, 0xB4, 0xB9, SDL_ALPHA_OPAQUE);
    SDL_FRect rect = {x, y, DIALOG_WIDTH, DIALOG_HEIGHT};
    SDL_RenderFillRect(ui_renderer, &rect);

    DialogueLine *line = &d->lines[d->bookmark];
    render_text_paragraph(line->text, x + LINE_HEIGHT, y + LINE_HEIGHT, 0);

    int option_start_y = y + LINE_HEIGHT + text_texture->h;
    if (line->replies != NULL) {
      for (int i = 0; i < line->replies_count; i++) {
        DialogueReply *reply = &line->replies[i];
        render_text_paragraph(reply->option_text, x + LINE_HEIGHT, option_start_y + ((i + 1) * LINE_HEIGHT), d->active_option == i);
        option_start_y += text_surface->h;
      }
    } else {
      DialogueReply *option = create_leave_option();
      render_text_paragraph(option->option_text, x + LINE_HEIGHT, option_start_y + LINE_HEIGHT, 1);
    }
  }
}

void free_ui() {
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_DestroyTexture(text_texture);
}

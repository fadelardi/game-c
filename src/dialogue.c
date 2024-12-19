#include "dialogue.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>

Dialogue *create_conversation(DialogueLine *lines, int line_count) {
  Dialogue *c;
  c = (Dialogue *)SDL_malloc(sizeof(Dialogue));
  c->lines = lines;
  c->count = line_count;
  c->bookmark = 0;
  c->active_option = 0;

  return c;
}

void set_active_option(Dialogue *d, int direction) {
  int new_option = d->active_option + direction;

  if (new_option < 0) {
    new_option = d->lines[d->bookmark].options_count - 1;
  } else if (new_option >= d->lines[d->bookmark].options_count) {
    new_option = 0;
  }

  d->active_option = new_option;
}

OptionAction trigger_active_option(Dialogue *d) {
  if (d != NULL && d->lines != NULL) {
    DialogueLine l = d->lines[d->bookmark];
    if (l.options != NULL && d->active_option < l.options_count) {
      return l.options[d->active_option].action;
    }
  }

  return OptionIgnore;
}

DialogueOption *create_leave_option(int option_count) {
  DialogueOption *opt;
  opt = (DialogueOption *)SDL_malloc(sizeof(DialogueOption));

  if (opt == NULL) {
    SDL_Log("Unable to create default option: %s", SDL_GetError());
    SDL_Quit();
  }

  char* text = (char *)SDL_malloc(8 * sizeof(char));
  if (text == NULL) {
    SDL_Log("Unable to allocate memory for text: %s", SDL_GetError());
    SDL_Quit();
  }
  text[0] = '\0';
  SDL_snprintf(text, 9, "%d", option_count);
  SDL_strlcat(text, ". Leave", 9);

  opt->option_text = text;
  opt->action = OptionExit;

  return opt;
}

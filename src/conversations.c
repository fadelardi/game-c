#include "conversations.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

Dialogue *conversations[NUM_CONVERSATIONS];

void init_conversations() {
  int total_lines = 1;
  int option_count = 2;
  DialogueLine *lines = (DialogueLine *)SDL_malloc(sizeof(DialogueLine) * total_lines);

  if (lines == NULL) {
    SDL_Log("Failed to allocate memory for conversation lines: %s", SDL_GetError());
    SDL_Quit();
    return;
  }

  DialogueOption *options = (DialogueOption *)SDL_malloc(sizeof(DialogueOption) * option_count);
  options[0].option_text = "1. Slap";
  options[0].action = OptionFight;
  options[1] = *create_leave_option(option_count);

  lines[0].text = "This is text for the initial conversation that goes into two lines.";
  lines[0].options_count = option_count;
  lines[0].options = options;
  conversations[0] = create_conversation(lines, total_lines);
}

void free_conversations() {
  for (int i = 0; i < NUM_CONVERSATIONS; i++) {
    SDL_free(conversations[i]);
  }
}

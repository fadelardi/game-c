#include "conversations.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static size_t conversation_count = 0;
Dialogue *conversations[NUM_CONVERSATIONS];

void add_conversation(char *line_texts[], char **option_texts[], const int *option_types[], const size_t option_counts[], const size_t num_lines) {
  Dialogue *dialogue = init_dialogue();
  DialogueLine *lines = init_dialogue_lines(num_lines);

  for(size_t i = 0; i < num_lines; i++) {
    DialogueReply *replies = init_replies(option_counts[i]);

    for (size_t j = 0; j < option_counts[i]; j++) {
      add_reply(replies, option_texts[i][j], option_types[i][j]);
    }

    add_dialogue_line(lines, line_texts[i], replies);
  }

  add_dialogue(dialogue, lines);
  conversations[conversation_count] = dialogue;
  conversation_count++;
}

void init_conversations() {
  const int LINE_COUNT = 2;
  char *lines[] = {
    "This is initial exposition. Player can only continue.",
    "This is the second line. Player has to make a choice."
  };
  char *options0_text[] = {"1. Continue"};
  const int option0_types[] = {ReplyContinue};
  
  char *options1_text[] = {"1. Slap", "2. Leave"};
  const int option1_types[] = {ReplyFight, ReplyExit};

  char **options_text[] = {options0_text, options1_text};
  const int *option_types[] = {option0_types, option1_types};
  const size_t option_counts[] = {1, 2};

  add_conversation(lines, options_text, option_types, option_counts, LINE_COUNT);
}

void free_conversations() {
  for (int i = 0; i < NUM_CONVERSATIONS; i++) {
    for (int j = 0; j < conversations[i]->line_count; j++) {
      if (conversations[i]->lines[j].replies != NULL) {
        SDL_free(conversations[i]->lines[j].replies);
      }
    }

    if (conversations[i]->lines != NULL) {
      SDL_free(conversations[i]->lines);
    }
    SDL_free(conversations[i]);
  }
}

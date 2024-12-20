#include "dialogue.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>

static size_t reply_count = 0;
static size_t line_count = 0;

Dialogue *init_dialogue() {
  Dialogue *d;
  d = (Dialogue *)SDL_malloc(sizeof(Dialogue));

  if (d == NULL) {
    SDL_Log("Unable to create conversation: %s", SDL_GetError());
    SDL_Quit();
  }

  return d;
}

void add_dialogue(Dialogue *d, DialogueLine *lines) {
  d->lines = lines;
  d->line_count = line_count;
  d->bookmark = 0;
  d->active_option = 0;
}

DialogueLine *init_dialogue_lines(const size_t total_lines) {
  DialogueLine *lines = (DialogueLine *)SDL_malloc(sizeof(DialogueLine) * total_lines);

  if (lines == NULL) {
    SDL_Log("Failed to allocate memory for conversation lines: %s", SDL_GetError());
    SDL_Quit();
  }

  return lines;
}

void add_dialogue_line(DialogueLine *lines, char *text, DialogueReply *replies) {
  lines[line_count].text = text;
  lines[line_count].replies = replies;
  lines[line_count].replies_count = reply_count;
  line_count++;
  reply_count = 0;
}

DialogueReply *init_replies(const size_t replies_total) {
  DialogueReply *replies = (DialogueReply *)SDL_malloc(sizeof(DialogueReply) * replies_total);

  if (replies == NULL) {
    SDL_Log("Failed to allocate memory for conversation replies: %s", SDL_GetError());
    SDL_Quit();
  }

  return replies;
}

void add_reply(DialogueReply *replies, char *text, ReplyType action) {
  replies[reply_count].option_text = text;
  replies[reply_count].action = action;
  reply_count++;
}

void set_active_option(Dialogue *d, int direction) {
  int new_option = d->active_option + direction;

  if (new_option < 0) {
    new_option = d->lines[d->bookmark].replies_count - 1;
  } else if (new_option >= d->lines[d->bookmark].replies_count) {
    new_option = 0;
  }

  d->active_option = new_option;
}

void move_conversation_forward(Dialogue *d) {
  if (d->bookmark < d->line_count - 1) {
    d->bookmark++;
    d->active_option = 0;
  }
}

ReplyType trigger_active_option(Dialogue *d) {
  if (d != NULL && d->lines != NULL) {
    DialogueLine l = d->lines[d->bookmark];
    if (l.replies != NULL && d->active_option < l.replies_count) {
      return l.replies[d->active_option].action;
    }
  }

  return ReplyContinue;
}

DialogueReply *create_leave_option(int reply_count) {
  DialogueReply *opt;
  opt = (DialogueReply *)SDL_malloc(sizeof(DialogueReply));

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
  SDL_snprintf(text, 9, "%d", reply_count);
  SDL_strlcat(text, ". Leave", 9);

  opt->option_text = text;
  opt->action = ReplyExit;

  return opt;
}

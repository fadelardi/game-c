#include "dialogue.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>

void add_dialogue(Dialogue *dialogue, DialogueLine *lines, size_t line_count) {
    dialogue->lines = lines;
    dialogue->line_count = line_count;
    dialogue->bookmark = 0;
    dialogue->active_option = 0;
}

void add_dialogue_line(DialogueLine *line, char *text, DialogueReply *replies, size_t replies_count, int line_idx) {
    line->text = SDL_strdup(text);
    line->replies = replies;
    line->replies_count = replies_count;
}

void add_reply(DialogueReply *reply, char *text, ReplyType action, int reply_idx) {
    reply->option_text = SDL_strdup(text);
    reply->action = action;
}

void add_conversation(Conversations *conversations, int idx, char *line_texts[],
                    char **replies_text[], const int *replies_types[],
                    const size_t replies_totals[], const size_t lines_total) {
    if (!conversations) {
        SDL_Log("add_conversation: Conversations is NULL");
        return;
    }
    if (idx < 0 || idx >= conversations->total_conversations) {
        SDL_Log("add_conversation: Index out of bounds");
        return;
    }
    Dialogue *dialogue = (Dialogue *)SDL_malloc(sizeof(Dialogue));
    DialogueLine *lines = (DialogueLine *)SDL_malloc(sizeof(DialogueLine) * lines_total);

    if (!dialogue || !lines) {
        SDL_Log("add_conversation: Failed to allocate memory");
        SDL_free(dialogue);
        SDL_free(lines);
        return;
    }

    for (size_t i = 0; i < lines_total; i++) {
        DialogueReply *replies = (DialogueReply *)SDL_malloc(sizeof(DialogueReply) * replies_totals[i]);

        if (!replies) {
            SDL_Log("add_conversation: Failed to allocate memory for replies");
            for(size_t k = 0; k < i; k++){
                SDL_free(lines[k].replies);
            }
            SDL_free(dialogue);
            SDL_free(lines);
            return;
        }

        for (size_t j = 0; j < replies_totals[i]; j++) {
            add_reply(&replies[j], replies_text[i][j], replies_types[i][j], j);
        }

        add_dialogue_line(&lines[i], line_texts[i], replies, replies_totals[i], i);
    }

    add_dialogue(dialogue, lines, lines_total);
    conversations->list[idx] = dialogue;
}

void init_conversations(Conversations **conversations, int num_conversations) {
  *conversations = (Conversations *)SDL_malloc(sizeof(Conversations));

  if ((*conversations) == NULL) {
    SDL_Log("Failed to allocate memory for conversations: %s", SDL_GetError());
    return;
  }

  (*conversations)->total_conversations = num_conversations;
  (*conversations)->list =
      (Dialogue **)SDL_malloc(sizeof(Dialogue *) * num_conversations);

  if ((*conversations)->list == NULL) {
    SDL_Log("Failed to allocate memory for conversation list: %s", SDL_GetError());
    SDL_free(*conversations); // Free previously allocated memory
    *conversations = NULL;
    return;
  }

  for (size_t i = 0; i < num_conversations; i++) {
    (*conversations)->list[i] = NULL;
  }
}

void free_conversations(Conversations *conversations) {
    if (!conversations) return;

    for (int i = 0; i < conversations->total_conversations; i++) {
        if (conversations->list[i]) {
            for (int j = 0; j < conversations->list[i]->line_count; j++) {
                if (conversations->list[i]->lines[j].replies) {
                    for(size_t k = 0; k < conversations->list[i]->lines[j].replies_count; k++){
                        SDL_free(conversations->list[i]->lines[j].replies[k].option_text);
                    }
                    SDL_free(conversations->list[i]->lines[j].replies);
                }
                SDL_free(conversations->list[i]->lines[j].text); //Free the strdup
            }
            SDL_free(conversations->list[i]->lines);
            SDL_free(conversations->list[i]);
        }
    }
    SDL_free(conversations->list);
    SDL_free(conversations);
}

void move_conversation(Dialogue *d, unsigned int bookmark) {
   if (d == NULL) {
    SDL_Log("Unable to move conversation");
    return;
  }

  SDL_Log("Moving conversation to %d / %lu", bookmark, d->line_count);
  if (bookmark < d->line_count) {
    d->bookmark = bookmark;
    d->active_option = 0;
  }
}

void move_conversation_forward(Dialogue *d) {
  if (d == NULL) {
    SDL_Log("Unable to move conversation forward: %s", SDL_GetError());
    return;
  }

  move_conversation(d, d->bookmark + 1);
}

ReplyType dialogue_pick_reply(Dialogue *d) {
  if (d == NULL || d->lines == NULL) {
    SDL_Log("Unable to trigger reply action");
    return ReplyContinue;
  }

  DialogueLine l = d->lines[d->bookmark];
  if (l.replies != NULL && d->active_option < l.replies_count) {
    return l.replies[d->active_option].action;
  }

  return ReplyContinue;
}

void dialogue_select_reply(Dialogue *d, int direction) {
  if (d == NULL || d->lines == NULL) {
    SDL_Log("Unable to select reply");
    return;
  }

  DialogueLine l = d->lines[d->bookmark];
  if (l.replies == NULL) {
    SDL_Log("Empty replies");
    return;
  }

  if (direction < 0 && d->active_option > 0) {
    d->active_option--;
  } else if (direction > 0 && d->active_option < l.replies_count - 1) {
    d->active_option++;
  }
}

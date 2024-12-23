#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <stddef.h>

typedef enum { ReplyExit, ReplyFight, ReplyContinue } ReplyType;

typedef struct {
  char *option_text;
  ReplyType action;
  int action_target;
} DialogueReply;

typedef struct {
  char *text;
  size_t replies_count;
  DialogueReply *replies;
} DialogueLine;

typedef struct {
  DialogueLine *lines;
  int bookmark;
  int active_option;
  size_t line_count;
} Dialogue;

typedef struct {
  Dialogue **list;
  size_t total_conversations;
} Conversations;

void init_conversations(Conversations **conversations, int num_conversations);
void add_conversation(Conversations *conversations, int idx, char *line_texts[],
                      char **replies_text[], const int *replies_types[],
                      const size_t replies_totals[], const size_t lines_total);
void free_conversations(Conversations *conversations);
Dialogue *init_dialogue();
void add_dialogue(Dialogue *dialogue, DialogueLine *lines, size_t line_count);
void add_dialogue_line(DialogueLine *lines, char *text, DialogueReply *replies, size_t replies_count, int line_idx);
void add_reply(DialogueReply *replies, char *text, ReplyType action, int reply_idx);
DialogueLine *init_dialogue_lines(size_t total_lines);
DialogueReply *init_replies(size_t total_replies);
ReplyType dialogue_pick_reply(Dialogue *d);
void move_conversation(Dialogue *d, unsigned int bookmark);
void move_conversation_forward(Dialogue *d);
void dialogue_select_reply(Dialogue *d, int direction);

#endif

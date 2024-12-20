#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <stddef.h>

typedef enum { ReplyExit, ReplyFight, ReplyContinue } ReplyType;

typedef struct {
  char *option_text;
  ReplyType action;
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

Dialogue *init_dialogue();
void add_dialogue(Dialogue *dialogue, DialogueLine *lines);
DialogueLine *init_dialogue_lines(size_t total_lines);
void add_dialogue_line(DialogueLine *line, char *text, DialogueReply *replies);
DialogueReply *init_replies(size_t total_replies);
void add_reply(DialogueReply *option, char *text, ReplyType action);
DialogueReply *create_leave_option();
ReplyType trigger_active_option(Dialogue *d);
void move_conversation_forward(Dialogue *d);
void set_active_option(Dialogue *d, int direction);

#endif

#ifndef DIALOGUE_H
#define DIALOGUE_H

typedef enum { OptionExit, OptionFight, OptionIgnore } OptionAction;

typedef struct {
  char *option_text;
  OptionAction action;
} DialogueOption;

typedef struct {
  char *text;
  int options_count;
  DialogueOption *options;
} DialogueLine;

typedef struct {
  DialogueLine *lines;
  int bookmark;
  int active_option;
  int count;
} Dialogue;

Dialogue *create_conversation(DialogueLine *lines, int line_count);
DialogueOption *create_leave_option();
OptionAction trigger_active_option(Dialogue *d);
void set_active_option(Dialogue *d, int direction);
void free_dialog(Dialogue *dialogue);

#endif

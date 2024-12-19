#ifndef CONVERSATIONS_H
#define CONVERSATIONS_H

#define NUM_CONVERSATIONS 1
#include "dialogue.h"

extern Dialogue *conversations[NUM_CONVERSATIONS];
void init_conversations();
void free_conversations();

#endif

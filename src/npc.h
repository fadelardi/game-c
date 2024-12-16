#include "base_entity.h"

typedef struct {
  BaseEntity base;
  int id;
} NPC;

typedef struct {
  NPC *npcs;
  int count;
} NPCArray;

NPC create_npc(int x, int y);
void update_npc(NPC *npc);
void add_npc(NPCArray *npc_array, NPC new_npc);
void remove_npcs(NPCArray *npc_array, int id);
void free_npcs(NPCArray *npc_array);

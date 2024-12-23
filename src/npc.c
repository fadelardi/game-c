#include "npc.h"
#include "constants.h"

NPC create_npc(int x, int y, const int CONV_IDX, Conversations *conversations) {
  NPC npc;

  Animation *idle_animation = create_animation(
      DEFAULT_CHAR_SIZE, DEFAULT_CHAR_SIZE, 6, WITCH2_IDLE_IDX, 1);
  Animation *walk_animation = create_animation(
      DEFAULT_CHAR_SIZE, DEFAULT_CHAR_SIZE, 6, WITCH2_WALK_IDX, 1);

  BaseEntity *b = init_base_entity(x, y, idle_animation, walk_animation);

  if (b == NULL) {
    return (NPC){0};
  }
  
  npc.base = *b;
  SDL_free(b);

  npc.dialogue = NULL;

  if (conversations != NULL) {
    if (CONV_IDX > -1 && CONV_IDX < conversations->total_conversations) {
      npc.dialogue = conversations->list[CONV_IDX];
    }
  } 
  
  return npc;
}

void add_npc(NPCArray *npc_array, NPC new_npc) {
  npc_array->npcs =
      (NPC *)SDL_realloc(npc_array->npcs, (npc_array->count + 1) * sizeof(NPC));

  if (!npc_array->npcs) {
    SDL_Log("Failed to reallocate memory when adding npcs");
    SDL_Quit();
  }

  npc_array->npcs[npc_array->count] = new_npc;
  npc_array->count++;
}

void remove_npcs(NPCArray *npc_array, int id) {
  for (int i = 0; i < npc_array->count; i++) {
    if (npc_array->npcs[i].id == id) {
      for (int j = i; j < npc_array->count - 1; j++) {
        npc_array->npcs[j] = npc_array->npcs[j + 1];
      }
      npc_array->count--;
      npc_array->npcs =
          (NPC *)SDL_realloc(npc_array->npcs, npc_array->count * sizeof(NPC));

      if (!npc_array->npcs && npc_array->count > 0) {
        SDL_Log("Failed to reallocate when deleting npcs: %s", SDL_GetError());
        SDL_Quit();
      }

      break;
    }
  }
}

void free_npcs(NPCArray *npc_array) { SDL_free(npc_array->npcs); }

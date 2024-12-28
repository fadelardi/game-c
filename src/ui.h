#include "dialogue.h"
#include "player_entity.h"
#include "npc.h"
#include <SDL3/SDL.h>

void init_ui(SDL_Renderer *renderer);
void render_dialogue(Dialogue *d);
void render_ui(SDL_Renderer *renderer, int npc_contact_idx, NPCArray npc_array, bool is_dialogue);
void render_map(SDL_Renderer *renderer);
void render_npcs(SDL_Renderer *renderer, NPCArray npc_array, PlayerEntity *player_entity, int *npc_contact_idx, bool is_dialogue);
void free_ui();

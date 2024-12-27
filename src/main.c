#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include "assets.h"
#include "player_entity.h"
#include "conversation_list.h"
#include "npc.h"
#include "ui.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static PlayerEntity *main_char = NULL;
static NPCArray npc_array = {NULL, 0};
static Conversations *conversations = NULL;
static SDL_Texture *ground = NULL;
static int npc_contact_idx = -1;
static enum GameStates { GAMEPLAY, DIALOGUE } game_state = GAMEPLAY;

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("Game C", WINDOW_WIDTH, WINDOW_HEIGHT, 0,
                                   &window, &renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  init_assets(renderer);
  init_ui(renderer);
  load_conversations(&conversations);
  main_char = create_char_entity(0, 0);

  NPC npc = create_npc(50, 50, 0, conversations);
  add_npc(&npc_array, npc);

  SDL_Surface *ground_img = IMG_Load("images/ground.png");
  ground = SDL_CreateTextureFromSurface(renderer, ground_img);
  SDL_DestroySurface(ground_img);

  return SDL_APP_CONTINUE;
}

void handle_gameplay_keys(SDL_Scancode code) {
  switch (code) {
  case SDL_SCANCODE_A:
    move_entity(main_char->base, D_LEFT);
    break;
  case SDL_SCANCODE_D:
    move_entity(main_char->base, D_RIGHT);
    break;
  case SDL_SCANCODE_W:
    move_entity(main_char->base, D_UP);
    break;
  case SDL_SCANCODE_S:
    move_entity(main_char->base, D_DOWN);
    break;
  case SDL_SCANCODE_Q:
    char_attack(main_char);
    break;
  case SDL_SCANCODE_E:
    if (npc_contact_idx > -1) {
      game_state = DIALOGUE;
    }
    break;
  case SDL_SCANCODE_ESCAPE:
    SDL_Quit();
    break;
  default:
    break;
  }
}

void handle_dialogue_keys(SDL_Scancode code) {
  int selected_option;

  switch (code) {
  case SDL_SCANCODE_W:
    dialogue_select_reply(npc_array.npcs[npc_contact_idx].dialogue, -1);
    break;
  case SDL_SCANCODE_S:
    dialogue_select_reply(npc_array.npcs[npc_contact_idx].dialogue, 1);
    break;
  case SDL_SCANCODE_E:
    selected_option =
        dialogue_pick_reply(npc_array.npcs[npc_contact_idx].dialogue);
    if (selected_option == ReplyExit || selected_option == ReplyFight) {
      game_state = GAMEPLAY;
    } else if (selected_option == ReplyContinue) {
      move_conversation_forward(npc_array.npcs[npc_contact_idx].dialogue);
    } else {
      unsigned int jump_to_index = selected_option * -1;
      move_conversation(npc_array.npcs[npc_contact_idx].dialogue,
                        jump_to_index);
    }
    break;
  default:
    break;
  }
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  } else if (event->type == SDL_EVENT_KEY_DOWN) {
    if (game_state == DIALOGUE) {
      handle_dialogue_keys(event->key.scancode);
    } else {
      handle_gameplay_keys(event->key.scancode);
    }
  } else if (event->type == SDL_EVENT_KEY_UP) {
    if (game_state == GAMEPLAY) {
      if (main_char->base->current_animation->key_down_activation) {
        reset_animation(main_char->base);
      }
    }
  }
  return SDL_APP_CONTINUE;
}

void render_player_char(SDL_Renderer *renderer) {
  update_entity(main_char->base);
  render_entity(main_char->base, renderer);
}

void render_npcs(SDL_Renderer *renderer) {
  int has_npc_contact = 0;

  for (int i = 0; i < npc_array.count; i++) {
    update_entity(&npc_array.npcs[i].base);
    render_entity(&npc_array.npcs[i].base, renderer);

    if (npc_contact_idx != -1 || !has_npc_contact) {
      const bool is_npc_hit = is_hitting_entity(main_char, &npc_array.npcs[i].base);
      if ((game_state == DIALOGUE && check_collision(main_char->base, &npc_array.npcs[i].base)) || (game_state == GAMEPLAY && is_npc_hit)) {
        npc_contact_idx = i;
        has_npc_contact = 1;
        if (is_npc_hit) {
          npc_array.npcs[i].base.current_animation = npc_array.npcs[i].base.dying_animation;
        }
        break;
      } else {
        npc_contact_idx = -1;
      }
    }
  }
}

void render_ui(SDL_Renderer *renderer, int npc_contact_idx) {
  if (npc_contact_idx > -1) {
    if (game_state == DIALOGUE) {
      render_dialogue(npc_array.npcs[npc_contact_idx].dialogue);
    }
  }
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  SDL_SetRenderDrawColor(renderer, 0xBB, 0xB1, 0xCC, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_RenderTexture(renderer, ground, NULL, NULL);

  render_npcs(renderer);
  render_player_char(renderer);
  render_ui(renderer, npc_contact_idx);

  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  free_main_char(main_char);
  free_npcs(&npc_array);
  free_assets();
  free_ui();
  free_conversations(conversations);
}

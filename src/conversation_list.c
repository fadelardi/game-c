#include "conversation_list.h"
#include <jansson.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

void load_reply(json_t *reply_json, char **reply_text, int *reply_type) {
    const char *text = json_string_value(json_object_get(reply_json, "text"));
    *reply_text = SDL_strdup(text);

    const char *type = json_string_value(json_object_get(reply_json, "type"));
    if (SDL_strcmp(type, "Continue") == 0) {
        *reply_type = ReplyContinue;
    } else if (SDL_strcmp(type, "Fight") == 0) {
        *reply_type = ReplyFight;
    } else if (SDL_strcmp(type, "Exit") == 0) {
        *reply_type = ReplyExit;
    } else {
        *reply_type = SDL_atoi(type);
    }
}

void load_line(json_t *line_json, char **line, char ***allocated_replies_text, int **allocated_replies_types, size_t *option_count) {
    const char *text = json_string_value(json_object_get(line_json, "text"));
    *line = SDL_strdup(text);

    json_t *replies_json = json_object_get(line_json, "replies");
    size_t replies_count = json_array_size(replies_json);
    *option_count = replies_count;
    *allocated_replies_text = (char **)SDL_malloc(sizeof(char *) * replies_count);
    *allocated_replies_types = (int *)SDL_malloc(sizeof(int) * replies_count);

    for (size_t j = 0; j < replies_count; j++) {
        json_t *reply_json = json_array_get(replies_json, j);
        load_reply(reply_json, &(*allocated_replies_text)[j], &(*allocated_replies_types)[j]);
    }
}


void load_conversation(Conversations *conversations, size_t conv_index, json_t *conversation) {
    size_t line_total = json_array_size(json_object_get(conversation, "lines"));
    char *lines[line_total];
    char **allocated_replies_text[line_total];
    int *allocated_replies_types[line_total];
    size_t reply_totals[line_total];
    json_t *lines_json = json_object_get(conversation, "lines");

    for (size_t i = 0; i < line_total; i++) {
        json_t *line_json = json_array_get(lines_json, i);
        load_line(line_json, &lines[i], &allocated_replies_text[i], &allocated_replies_types[i], &reply_totals[i]);
    }

    add_conversation(conversations, conv_index, lines, allocated_replies_text, (const int **)allocated_replies_types, reply_totals, line_total);

    for (size_t i = 0; i < line_total; i++) {
        for (size_t j = 0; j < reply_totals[i]; j++) {
            SDL_free(allocated_replies_text[i][j]);
        }
        SDL_free(allocated_replies_text[i]);
        SDL_free(allocated_replies_types[i]);
        SDL_free(lines[i]);
    }
}

void load_conversations(Conversations **conversations) {
    json_error_t error;
    json_t *root = json_load_file("src/conversations.json", 0, &error);
    if (!root) {
        SDL_Log("Error loading conversations.json: %s", error.text);
        return;
    }

    size_t num_conversations = json_array_size(root);
    init_conversations(conversations, num_conversations);

    (*conversations)->total_conversations = num_conversations;
    for (size_t conv_index = 0; conv_index < num_conversations; conv_index++) {
        json_t *conversation = json_array_get(root, conv_index);
        load_conversation(*conversations, conv_index, conversation);
    }

    json_decref(root); // Release JSON data
    SDL_Log("loaded convos successfully");
}

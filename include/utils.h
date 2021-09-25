#ifndef UTILS_H
#define UTILS_H

#include <string.h>

#include "node.h"
#include "token.h"
#include "queue.h"

int char_to_bool(char* str);
void create_token_queue();
void push_token_queue(void* token);
void clear_token_queue();

void exporta (void *arvore);
void libera (void *arvore);

#endif
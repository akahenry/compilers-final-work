/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#ifndef UTILS_H
#define UTILS_H

#include <string.h>

typedef struct _node_t node_t;

#include "node.h"
#include "queue.h"
#include "symbol_table.h"

int char_to_bool(char* str);
void create_token_queue();
void push_token_queue(void* token);
void clear_token_queue();

void create_symbol_table();
void open_scope();
void close_scope();
void add_symbol(char* key, node_t* node);

void exporta (void *arvore);
void libera (void *arvore);

#endif
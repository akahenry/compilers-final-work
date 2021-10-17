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
#include "errors.h"

int char_to_bool(char* str);
void create_token_queue();
token_t* create_token(int line, token_type_t type, char* value);
void push_token_queue(void* token);
void clear_token_queue();

void create_symbol_table();
void print_symbol_table();
void open_scope();
void close_scope();
void add_symbol(char* key, token_t* token, symbol_type_t type, symbol_datatype_t datatype, token_t* vector_size_token);
symbol_item_t* get_symbol(char* key);
symbol_item_t* get_symbol_lit_char(char* key);
symbol_item_t* get_symbol_lit_string(char* key);
const char* datatype_string(symbol_datatype_t datatype);

void exporta (void *arvore);
void libera (void *arvore);

#endif
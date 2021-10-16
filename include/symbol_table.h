/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "hash.h"
#include "stack.h"
#include "token.h"

#define SCOPE_TABLE_SIZE 1024

typedef enum _symbol_error_t {
    SYMBOL_NO_ERROR,
    SYMBOL_ERROR_KEY_ALREADY_EXISTS
} symbol_error_t;

typedef enum _symbol_datatype_t {
    SYMBOL_DATATYPE_INT,
    SYMBOL_DATATYPE_FLOAT,
    SYMBOL_DATATYPE_BOOL,
    SYMBOL_DATATYPE_CHAR,
    SYMBOL_DATATYPE_STRING
} symbol_datatype_t;

typedef enum _symbol_type_t {
    SYMBOL_TYPE_LITERAL_INT,
    SYMBOL_TYPE_LITERAL_FLOAT,
    SYMBOL_TYPE_LITERAL_BOOL,
    SYMBOL_TYPE_LITERAL_CHAR,
    SYMBOL_TYPE_LITERAL_STRING,
    SYMBOL_TYPE_IDENTIFIER_VARIABLE,
    SYMBOL_TYPE_IDENTIFIER_FUNCTION    
} symbol_type_t;

typedef struct _symbol_item_t {
    int line_number;
    symbol_type_t type;
    symbol_datatype_t datatype;
    token_t* token;
    size_t size;
} symbol_item_t;

typedef struct _symbol_table_t {
    stack_t* scopes;
    size_t size;
} symbol_table_t;

symbol_table_t* symbol_table_create();
symbol_error_t symbol_table_add_symbol(symbol_table_t* table, char* key, int line_number, symbol_type_t type, symbol_datatype_t datatype, size_t size, token_t* token);
symbol_item_t* symbol_table_get_symbol(symbol_table_t* table, char* key);
void symbol_table_open_scope(symbol_table_t* table);
void symbol_table_close_scope(symbol_table_t* table);
void symbol_table_print(symbol_table_t* table);
void symbol_table_destroy(symbol_table_t* table);

#endif
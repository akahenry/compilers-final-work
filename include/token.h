/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>
#include <string.h>

typedef union _token_value_t {
    int v_integer;
    float v_float;
    int v_bool;
    char v_char;
    char *v_string;
} token_value_t;  

typedef enum _token_type_t {
	TOKEN_TYPE_LITERAL_INT,
    TOKEN_TYPE_LITERAL_FLOAT,
    TOKEN_TYPE_LITERAL_BOOL,
    TOKEN_TYPE_LITERAL_CHAR,
    TOKEN_TYPE_LITERAL_STRING,
    TOKEN_TYPE_SPECIAL_CHARACTER,
    TOKEN_TYPE_COMPOSITE_OPERATOR,
    TOKEN_TYPE_IDENTIFIER
} token_type_t;

typedef struct _token_t
{
    int line;   // Line number of occurrence;
    token_type_t type;
    token_value_t value;
    char* text;
} token_t;

token_t* token_create(int _line, token_type_t _type, char* _value);
void token_destroy(token_t* token);

#endif
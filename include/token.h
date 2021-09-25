#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
	TOKEN_TYPE_SPECIAL_CHARACTER,
    TOKEN_TYPE_COMPOSITE_OPERATOR,
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_LITERAL,
} token_type_t;

typedef struct _token_t
{
    int line;   // Line number of occurrence;
    token_type_t type;
    char* value;
} token_t;

#endif
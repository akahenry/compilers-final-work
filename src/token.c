#include "token.h"

token_t* createToken(int _line, token_type_t _type, char* _value)
{
    token_t* token = calloc(1, sizeof(token_t));

    token->line = _line;
    token->type = _type;
    strcpy(token->value, _value);

    return token;
}
/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#include "token.h"

#include "utils.h"

token_t* token_create(int _line, token_type_t _type, char* _value)
{
    token_t* token = calloc(1, sizeof(token_t));

    token->line = _line;
    token->type = _type;
	token->text = strdup(_value);

	switch(_type) {
		case TOKEN_TYPE_LITERAL_INT:
			token->value.v_integer = atoi(_value);
			break;

		case TOKEN_TYPE_LITERAL_FLOAT:
			token->value.v_float = atof(_value);
			break;

		case TOKEN_TYPE_LITERAL_BOOL:
            token->value.v_bool = char_to_bool(_value);		
			break;

		case TOKEN_TYPE_LITERAL_CHAR:
			token->value.v_char = _value[1];
			break;

		case TOKEN_TYPE_LITERAL_STRING:
			token->value.v_string = strdup(_value);
			break;

		case TOKEN_TYPE_IDENTIFIER:
			token->value.v_string = strdup(_value);
			break;
	}

    return token;
}

void token_destroy(token_t* token)
{
	if (token != NULL)
	{
		free(token->text);
		if (token->type == TOKEN_TYPE_IDENTIFIER || token->type == TOKEN_TYPE_LITERAL_STRING)
		{
			free(token->value.v_string);
		}
		free(token);
	}
}
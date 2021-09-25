#include "token.h"

token_t* create_token(int _line, token_type_t _type, char* _value)
{
	create_token_queue();
    token_t* token = calloc(1, sizeof(token_t));
	push_token_queue((void*)token);

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
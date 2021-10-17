/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#include "utils.h"

queue_t* q_alloc_token = NULL;
symbol_table_t* symbol_table = NULL;

int char_to_bool(char* str)
{
    if(!strcmp(str, "false"))
        return 0;
    else if(!strcmp(str, "true"))
        return 1;
    else
        return -1;	
}

void create_token_queue()
{
    if (q_alloc_token == NULL)
        q_alloc_token = queue_create();
}

token_t* create_token(int line, token_type_t type, char* value)
{
    create_token_queue();
    token_t* token = token_create(line, type, value);
    push_token_queue(token);

    return token;
}

void create_symbol_table()
{
    if (symbol_table == NULL)
    {
        symbol_table = symbol_table_create();
        symbol_table_open_scope(symbol_table); // Create global scope
    }
}

void print_symbol_table()
{
    if (symbol_table != NULL)
    {
        symbol_table_print(symbol_table);
    }
}

void open_scope()
{
    create_symbol_table();

    symbol_table_open_scope(symbol_table);
}

void close_scope()
{
    create_symbol_table();

    symbol_table_close_scope(symbol_table);
}

void add_symbol(char* key, token_t* token, symbol_type_t type, symbol_datatype_t datatype, token_t* vector_size_token)
{
    if (token != NULL)
    {
        if (symbol_table == NULL)
        {
            create_symbol_table();
        }

        size_t size = 0;
        char* label = key;

        switch (datatype)
        {
            case SYMBOL_DATATYPE_INT:
                size = 4;
                break;
            case SYMBOL_DATATYPE_FLOAT:
                size = 8;
                break;
            case SYMBOL_DATATYPE_BOOL:
                size = 1;
                break;
            case SYMBOL_DATATYPE_CHAR:
                size = 1;
                break;
            case SYMBOL_DATATYPE_STRING:
                if (type == SYMBOL_TYPE_IDENTIFIER_FUNCTION || type == SYMBOL_TYPE_IDENTIFIER_VARIABLE)
                {
                    size = 0; // O tamanho de variáveis do tipo string é definido na inicialização da variável
                }
                else if (type == SYMBOL_TYPE_LITERAL_STRING)
                {
                    size = strlen(token->value.v_string) + 1;
                }
                break;
            default:
                break;
        }
        
        if (type == SYMBOL_TYPE_LITERAL_CHAR || type == SYMBOL_TYPE_LITERAL_STRING)
        {
            label = calloc(strlen(key) + 2, sizeof(char));
            sprintf(label, "%d%s", type, key);
        }

        if (vector_size_token != NULL)
        {
            if (datatype != SYMBOL_DATATYPE_STRING)
            {
                size = size * vector_size_token->value.v_integer;
            }
            else
            {
                fprintf(stderr, "Semantic Error: vector `%s`, declared on line %d, is of type `string` and vectors cannot be of type `string`", token->text, token->line);
                exit(ERR_STRING_VECTOR);
            }
        }

        if (symbol_table_add_symbol(symbol_table, label, token->line, type, datatype, size, token, NULL) == SYMBOL_ERROR_KEY_ALREADY_EXISTS &&
            (type == SYMBOL_TYPE_IDENTIFIER_VARIABLE || type == SYMBOL_TYPE_IDENTIFIER_FUNCTION || type == SYMBOL_TYPE_IDENTIFIER_VECTOR))
        {
            symbol_item_t* declared_symbol = get_symbol(key);
            fprintf(stderr, "Semantic Error: duplicated identifier `%s` declaration on line %d and %d", key, declared_symbol->line_number, token->line);
            exit(ERR_DECLARED);
        }
        if (label != key)
        {
            free(label);
        }
    }
}

symbol_item_t* get_symbol(char* key)
{
    if (key != NULL)
    {
        return symbol_table_get_symbol(symbol_table, key);
    }
    else 
    {
        return NULL;
    }
}

symbol_item_t* get_symbol_lit_char(char* key)
{
    char* label = calloc(strlen(key) + 2, sizeof(char));
    sprintf(label, "%d%s", SYMBOL_TYPE_LITERAL_CHAR, key);
    symbol_item_t* response = get_symbol(label);
    free(label);
    return response;
}

symbol_item_t* get_symbol_lit_string(char* key)
{
    char* label = calloc(strlen(key) + 2, sizeof(char));
    sprintf(label, "%d%s", SYMBOL_TYPE_LITERAL_STRING, key);
    symbol_item_t* response = get_symbol(label);
    free(label);
    return response;
}

const char* datatype_string(symbol_datatype_t datatype)
{
    switch (datatype)
    {
        case SYMBOL_DATATYPE_INT:
            return datatype_string_int;
            break;
        case SYMBOL_DATATYPE_FLOAT:
            return datatype_string_float;
            break;
        case SYMBOL_DATATYPE_BOOL:
            return datatype_string_bool;
            break;
        case SYMBOL_DATATYPE_CHAR:
            return datatype_string_char;
            break;
        case SYMBOL_DATATYPE_STRING:
            return datatype_string_string;
            break;
        default:
            break;
    }
}

void push_token_queue(void* token)
{
    queue_push(q_alloc_token, token);
}

void clear_token_queue()
{
    free(q_alloc_token);
}

void exporta(void* tree)
{
    if (tree == NULL)
    {
        return;
    }
    
    node_t* node_ptr = (node_t*) tree;
    printf("%p [label=\"%s\"];\n", node_ptr, node_ptr->label);


    if (node_ptr->child1 != NULL)
    {
        printf("%p, %p\n", node_ptr, node_ptr->child1);
        exporta(node_ptr->child1);
    }
    if (node_ptr->child2 != NULL)
    {
        printf("%p, %p\n", node_ptr, node_ptr->child2);
        exporta(node_ptr->child2);
    }
    if (node_ptr->child3 != NULL)
    {
        printf("%p, %p\n", node_ptr, node_ptr->child3);
        exporta(node_ptr->child3);
    }
    if (node_ptr->child4 != NULL)
    {
        printf("%p, %p\n", node_ptr, node_ptr->child4);
        exporta(node_ptr->child4);
    }
    if (node_ptr->child5 != NULL)
    {
        printf("%p, %p\n", node_ptr, node_ptr->child5);
        exporta(node_ptr->child5);
    }
}

void libera(void *tree)
{
    node_t* node_ptr = (node_t*) tree;
    node_destroy(node_ptr);

    if (q_alloc_token != NULL)
    {
        while(!queue_empty(q_alloc_token)) {
            token_t* token = (token_t*)queue_pop(q_alloc_token);
            token_destroy(token);
        }
        queue_destroy(q_alloc_token);
    }

    if (symbol_table != NULL)
    {
        symbol_table_destroy(symbol_table);
    }
}
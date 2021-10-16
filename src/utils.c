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

void create_symbol_table()
{
    if (symbol_table == NULL)
    {
        symbol_table = symbol_table_create();
        symbol_table_open_scope(symbol_table); // Create global scope
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

void add_symbol(char* key, node_t* node)
{
    if (node != NULL && node->token != NULL)
    {
        if (symbol_table == NULL)
            create_symbol_table();

        symbol_type_t s_type;
        size_t size;
        switch (node->token->type)
        {
        case TOKEN_TYPE_IDENTIFIER:
            if (strncmp("call ", node->label, strlen("call ")) == 0)
            {
                s_type = SYMBOL_TYPE_IDENTIFIER_FUNCTION;
            }
            else
            {
                s_type = SYMBOL_TYPE_IDENTIFIER_VARIABLE;
            }
            break;
        default:
            s_type = (symbol_type_t)node->token->type;
            break;
        }

        switch (node->type)
        {
            case NODE_TYPE_INT:
                size = 4;
                break;
            case NODE_TYPE_FLOAT:
                size = 8;
                break;
            case NODE_TYPE_BOOL:
                size = 1;
                break;
            case NODE_TYPE_CHAR:
                size = 1;
                break;
            case NODE_TYPE_STRING:
                size = strlen(node->token->value.v_string) + 1;
                break;
            default:
                break;
        }

        symbol_table_add_symbol(symbol_table, key, node->token->line, s_type, (symbol_datatype_t)node->type, size, node->token);
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
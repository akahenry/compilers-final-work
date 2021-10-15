#include "symbol_table.h"

symbol_table_t* symbol_table_create()
{
    stack_t* stack = start_stack();
    symbol_table_t* table = calloc(1, sizeof(symbol_table_t));
    table->scopes = stack;
    table->size = 0;

    return table;
}

void symbol_table_add_symbol(symbol_table_t* table, char* key, int line_number, symbol_type_t type, symbol_datatype_t datatype, size_t size, token_t* token)
{
    if (table != NULL)
    {
        hash_table_t* scope = (hash_table_t*)stack_pop(table->scopes);
        if (scope != NULL)
        {
            symbol_item_t* item = calloc(1, sizeof(symbol_item_t));
            item->line_number = line_number;
            item->type = type;
            item->datatype = datatype;
            item->size = size;
            item->token = token;
            
            hash_add(scope, key, (void*)item);
        }
        stack_push(table->scopes, (void*)scope);
    }
}

void symbol_table_get_symbol(symbol_table_t* table, char* key)
{
    if (table != NULL)
    {
        queue_t* queue = start_queue();

        for (size_t i = 0; i < table->size; i++)
        {
            hash_table_t* scope = (hash_table_t*)stack_pop(table->scopes);
            queue_push(queue, (void*)scope);
            hash_get(scope, key);
        }
    }
}

void symbol_table_open_scope(symbol_table_t* table)
{
    if (table != NULL)
    {
        hash_table_t* scope = calloc(1, sizeof(hash_table_t));
        table->size++;
        stack_push(table->scopes, (void*)scope);
    }
}

void symbol_table_close_scope(symbol_table_t* table)
{
    if (table != NULL)
    {
        hash_table_t* scope = (hash_table_t*)stack_pop(table->scopes);
        if (scope != NULL)
        {
            hash_destroy(scope);
        }
        table->size--;
    }
}

void symbol_table_destroy(symbol_table_t* table)
{
    if (table != NULL)
    {        
        while (table->size > 0)
        {
            hash_table_t* scope = (hash_table_t*)stack_pop(table->scopes);

            if (scope != NULL)
            {
                for (size_t i = 0; i < scope->size; i++)
                {
                    free(scope->items[i]);
                }
                hash_destroy(scope);
                table->size--;
            }
        }
        clear_stack(table->scopes);
        free(table);
    }
}
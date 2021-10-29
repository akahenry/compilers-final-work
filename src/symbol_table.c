#include "symbol_table.h"

symbol_table_t* symbol_table_create()
{
    stack_t* stack = stack_create();
    symbol_table_t* table = calloc(1, sizeof(symbol_table_t));
    table->scopes = stack;
    table->size = 0;

    return table;
}

symbol_error_t symbol_table_add_symbol(symbol_table_t* table, char* key, int line_number, symbol_type_t type, symbol_datatype_t datatype, size_t size, token_t* token, queue_t* params_queue)
{
    symbol_error_t response = SYMBOL_NO_ERROR;
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
            item->params_queue = params_queue;
            
            if (hash_add(scope, key, (void*)item) == HASH_ERROR_KEY_ALREADY_EXISTS)
            {
                response = SYMBOL_ERROR_KEY_ALREADY_EXISTS;
                free(item);
            }
        }
        stack_push(table->scopes, (void*)scope);
    }
    return response;
}

symbol_item_t* symbol_table_get_symbol(symbol_table_t* table, char* key)
{
    symbol_item_t* item = NULL;
    if (table != NULL)
    {
        stack_t* stack = stack_create();

        while(!stack_empty(table->scopes))
        {
            hash_table_t* scope = (hash_table_t*)stack_pop(table->scopes);
            stack_push(stack, (void*)scope);
            item = hash_get(scope, key);
            if (item != NULL)
            {
                break;
            }
        }

        while (!stack_empty(stack))
        {
            stack_push(table->scopes, stack_pop(stack));
        }
        stack_destroy(stack);
    }

    return item;
}

void symbol_table_open_scope(symbol_table_t* table)
{
    if (table != NULL)
    {
        hash_table_t* scope = hash_create(SCOPE_TABLE_SIZE);
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
            for (size_t i = 0; i < scope->size; i++)
            {
                if (scope->items[i] != NULL)
                {
                    symbol_item_t* item = (symbol_item_t*)scope->items[i];
                    if (item->params_queue != NULL)
                    {
                        while(!queue_empty(item->params_queue))
                        {
                            free(queue_pop(item->params_queue));
                        }
                        queue_destroy(item->params_queue);
                    }
                    free(item);
                }
            }
            hash_destroy(scope);
            table->size--;
        }
    }
}

void symbol_table_print(symbol_table_t* table)
{
    if (table != NULL)
    {
        stack_t* stack = stack_create();
        int scope_number = 0;
        while(!stack_empty(table->scopes))
        {
            hash_table_t* scope = (hash_table_t*)stack_pop(table->scopes);
            stack_push(stack, (void*)scope);
            printf("Scope %d\n", scope_number);
            hash_print(scope);
            scope_number++;
        }
        printf("\n");

        while (!stack_empty(stack))
        {
            stack_push(table->scopes, stack_pop(stack));
        }
        stack_destroy(stack);
    }
}

void symbol_table_destroy(symbol_table_t* table)
{
    if (table != NULL)
    {        
        while (!stack_empty(table->scopes))
        {
            hash_table_t* scope = (hash_table_t*)stack_pop(table->scopes);

            if (scope != NULL)
            {
                for (size_t i = 0; i < scope->size; i++)
                {
                    if (scope->items[i] != NULL)
                    {
                        symbol_item_t* item = (symbol_item_t*)scope->items[i];
                        if (item->params_queue != NULL)
                        {
                            while(!queue_empty(item->params_queue))
                            {
                                free(queue_pop(item->params_queue));
                            }
                            queue_destroy(item->params_queue);
                        }
                        free(item);
                    }
                }
                hash_destroy(scope);
                table->size--;
            }
        }
        stack_destroy(table->scopes);
        free(table);
    }
}
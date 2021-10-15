/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#include "hash.h"

unsigned long hash_function(char* str, int size) 
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % size;
}

hash_table_t* hash_create(int size)
{
    void** items = calloc(size, sizeof(void*));
    hash_table_t* table = calloc(1, sizeof(hash_table_t));
    table->items = items;
    table->size = size;

    return table;
}

void* hash_get(hash_table_t* table, char* key)
{
    if (table != NULL && key != NULL)
    {
        return table->items[hash_function(key, table->size)];
    }
    else
    {
        return NULL;
    }
}

hash_error_t hash_add(hash_table_t* table, char* key, void* value)
{
    if (hash_get(table, key) != NULL)
    {
        return HASH_ERROR_KEY_ALREADY_EXISTS;
    }
    else if (table != NULL && key != NULL)
    {
        table->items[hash_function(key, table->size)] = value;
        return 0;
    }
}

hash_error_t hash_remove(hash_table_t* table, char* key)
{
    if (hash_get(table, key) != NULL)
    {
        table->items[hash_function(key, table->size)] = NULL;
        return 0;
    }
    else
    {
        return HASH_ERROR_KEY_DOES_NOT_EXIST;
    }
}

void hash_destroy(hash_table_t* table)
{
    if (table != NULL)
    {
        free(table->items);
        free(table);
    } 
}

void hash_print(hash_table_t* table)
{
    if (table != NULL)
    {
        for (size_t i = 0; i < table->size; i++)
        {
            printf("Key: %ld, Value: %p\n", i, table->items[i]);
        }
    }
}
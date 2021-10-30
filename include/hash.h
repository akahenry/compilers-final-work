/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#ifndef HASH_H
#define HASH_H

#include <stdlib.h>
#include <stdio.h>

typedef enum _hash_error_t
{
    HASH_NO_ERROR,
    HASH_ERROR_KEY_ALREADY_EXISTS,
    HASH_ERROR_KEY_DOES_NOT_EXIST
} hash_error_t;

typedef struct _hash_table_t
{
    void** items;
    int size;
    uint disp;
} hash_table_t;

hash_table_t* hash_create(int size);
void hash_destroy(hash_table_t* table);
hash_error_t hash_add(hash_table_t* table, char* key, void* value);
hash_error_t hash_remove(hash_table_t* table, char* key);
void* hash_get(hash_table_t* table, char* key);
void hash_print(hash_table_t* table);

#endif
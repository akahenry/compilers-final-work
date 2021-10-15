/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

typedef struct _stack_item_t {
	void* value;
	struct _stack_item_t* next;
} stack_item_t;

typedef struct _stack_t {
    stack_item_t* begin;
	stack_item_t* end;
} stack_t;

stack_item_t* new_stack_item(void* value);

stack_t* start_stack();
void stack_push(stack_t* s, void* value);
void* stack_pop(stack_t* s);
void* stack_get(stack_t* s);
int is_empty_stack(stack_t* s);
void clear_stack(stack_t* s);

#endif
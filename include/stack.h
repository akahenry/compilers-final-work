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
	size_t size;
} stack_t;

stack_item_t* stack_new_item(void* value);

stack_t* stack_create();
void stack_push(stack_t* s, void* value);
void* stack_pop(stack_t* s);
void* stack_get(stack_t* s);
void* stack_at(stack_t* q, int index);
int stack_empty(stack_t* s);
void stack_destroy(stack_t* s);

#endif
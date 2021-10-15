/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#include "stack.h"

stack_t* start_stack() 
{
	stack_t* s = calloc(1, sizeof(stack_t));

	s->begin = NULL;
	s->end = NULL;
	return s;
}

stack_item_t* new_stack_item(void* value) 
{
	stack_item_t *i = calloc(1, sizeof(stack_item_t));

	i->value = value;
	i->next = NULL;
	return i;
}

void stack_push(stack_t* s, void* value) 
{
	stack_item_t* new_item = new_stack_item(value);

	if(s->begin == NULL) 
	{
		s->begin = new_item;
		s->end = new_item;
		return;
	}

	new_item->next = s->begin;
	s->begin = new_item;
}

void* stack_pop(stack_t* s) 
{
	if(s->begin == NULL)
		return NULL;

	stack_item_t* i = s->begin;
	void* response = s->begin->value;
	s->begin = s->begin->next;

	if(s->begin == NULL)
		s->end = NULL;

	free(i);
	return response;
}

void* stack_get(stack_t* s)
{
	if (s->begin == NULL)
		return NULL;

	return s->begin->value;
}

int is_empty_stack(stack_t* s) 
{
	if(s->end == NULL && s->begin == NULL)
		return 1;
	return 0;
}

void clear_stack(stack_t* s) 
{
	stack_item_t* i = s->begin;

	while(i != NULL) 
	{
		i = i->next;
		free(s->begin);
		s->begin = i;
	}
	free(s);
}
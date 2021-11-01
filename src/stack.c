/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#include "stack.h"

stack_t* stack_create() 
{
	stack_t* s = calloc(1, sizeof(stack_t));

	s->begin = NULL;
	s->end = NULL;
	s->size = 0;
	return s;
}

stack_item_t* stack_new_item(void* value) 
{
	stack_item_t *i = calloc(1, sizeof(stack_item_t));

	i->value = value;
	i->next = NULL;
	return i;
}

void stack_push(stack_t* s, void* value) 
{
	stack_item_t* new_item = stack_new_item(value);
	s->size++;

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
	s->size--;
	return response;
}

void* stack_get(stack_t* s)
{
	if (s->begin == NULL)
		return NULL;

	return s->begin->value;
}

void* stack_at(stack_t* s, int index)
{
	stack_item_t* item = NULL;
	void* response = NULL;
	if (s != NULL)
	{
		if (index >= 0 && index < s->size)
		{
			item = s->begin;
			for (size_t i = 1; i <= index; i++)
			{
				item = item->next;
			}
			response = item->value;
		}
	}

	return response;
}

int stack_empty(stack_t* s) 
{
	if(s->end == NULL && s->begin == NULL)
		return 1;
	return 0;
}

void stack_destroy(stack_t* s) 
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
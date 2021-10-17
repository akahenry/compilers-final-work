/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#include "queue.h"

queue_t* queue_create() 
{
	queue_t* q = calloc(1, sizeof(queue_t));

	q->begin = NULL;
	q->end = NULL;
	q->size = 0;
	return q;
}

queue_item_t* queue_new_item(void* value) 
{
	queue_item_t *i = calloc(1, sizeof(queue_item_t));

	i->value = value;
	i->next = NULL;
	return i;
}

void queue_push(queue_t* q, void* value) 
{
	queue_item_t* new_item = queue_new_item(value);
	q->size++;

	if(q->end == NULL) 
	{
		q->begin = new_item;
		q->end = new_item;
		return;
	}

	q->end->next = new_item;
	q->end = new_item;
}

void* queue_pop(queue_t* q) 
{
	if(q->begin == NULL)
		return NULL;

	queue_item_t* i = q->begin;
	void* response = q->begin->value;
	q->begin = q->begin->next;

	if(q->begin == NULL)
		q->end = NULL;

	free(i);
	q->size--;
	return response;
}

void* queue_at(queue_t* q, int index)
{
	queue_item_t* item = NULL;
	void* response = NULL;
	if (q != NULL)
	{
		if (index >= 0 && index < q->size)
		{
			item = q->begin;
			for (size_t i = 1; i <= index; i++)
			{
				item = item->next;
			}
			response = item->value;
		}
	}

	return response;
}

int queue_empty(queue_t* q) 
{
	if(q->end == NULL && q->begin == NULL)
		return 1;
	return 0;
}

void queue_destroy(queue_t* q) 
{
	queue_item_t* i = q->begin;

	while(i != NULL) 
	{
		i = i->next;
		free(q->begin);
		q->begin = i;
	}
	free(q);
}
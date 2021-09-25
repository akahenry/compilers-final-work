#include "queue.h"

queue_t* start_queue() 
{
	queue_t* q = calloc(1, sizeof(queue_t));

	q->begin = NULL;
	q->end = NULL;
	return q;
}

queue_item_t* new_queue_item(void* value) 
{
	queue_item_t *i = calloc(1, sizeof(queue_item_t));

	i->value = value;
	i->next = NULL;
	return i;
}

void queue_push(queue_t* q, void* value) 
{
	queue_item_t* new_item = new_queue_item(value);

	if(q->end == NULL) 
	{
		q->begin = new_item;
		q->end = new_item;
		return;
	}

	q->end->next = new_item;
	q->end = new_item;
}

queue_item_t* queue_pop(queue_t* q) 
{
	if(q->begin == NULL)
		return NULL;

	queue_item_t* i = q->begin;
	q->begin = q->begin->next;

	if(q->begin == NULL)
		q->end = NULL;

	return i;
}

int is_empty_queue(queue_t* q) 
{
	if(q->end == NULL && q->begin == NULL)
		return 1;
	return 0;
}

void clear_queue(queue_t* q) 
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
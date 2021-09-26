/*
Grupo D
  Henry Bernardo Kochenborger de Avila
  Marcos Samuel Winkel Landi
*/
#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

typedef struct _queue_item_t {
	void* value;
	struct _queue_item_t* next;
} queue_item_t;

typedef struct _queue_t {
	queue_item_t* begin;
	queue_item_t* end;
} queue_t;

queue_item_t* new_queue_item(void* value);

queue_t* start_queue();
void queue_push(queue_t* q, void* value);
queue_item_t* queue_pop(queue_t* q);
int is_empty_queue(queue_t* q);
void clear_queue(queue_t* q);

#endif
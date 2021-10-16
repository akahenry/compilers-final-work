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

queue_item_t* queue_new_item(void* value);

queue_t* queue_create();
void queue_push(queue_t* q, void* value);
void* queue_pop(queue_t* q);
int queue_empty(queue_t* q);
void queue_destroy(queue_t* q);

#endif
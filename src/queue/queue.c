#include <stdlib.h>
#include <stdio.h>
#include "internal/queue.h"
#include "internal/elem.h"

queue_t *queue_create()
{
  queue_t *qptr = malloc(sizeof(queue_t));
  if (qptr == NULL)
  {
    printf("simple_queue_t* pointer is null!\n");
    exit(EXIT_FAILURE);
  }
  qptr->size = 0;

  return qptr;
}

elem_t *queue_pop(queue_t *q)
{
  if(q == NULL)
  {
    printf("popping from a NULL simple_queue_t\n");
    return NULL;
  }
  if(q->size == 0)
  {
    printf("popping a simple_queue_t which is empty!\n");
    return NULL;
  }

  q->size -= 1;
  elem_t *pop_this = q->first;
  q->first = pop_this->next;
  return pop_this;
}

void queue_push(queue_t *q, elem_t *e)
{
  if(q == NULL)
  {
    printf("pushing a NULL simple_queue_t\n");
    return;
  }

  if (e == NULL)
  {
    printf("push_q element is null!\n");
    return;
  }

  if (q->first == NULL)
  {
    printf("push_q first element is null, adding!\n");
    q->first = e;
    q->last  = e;
    q->size += 1;
    return;
  }
  
  q->last->next  = e;
  q->last        = e;
  q->size       += 1;
  return;
}

void queue_destroy(queue_t *q)
{
  elem_t *e;
  while( q->size > 0)
  {
    e = queue_pop(q);
    free(e);
  }
  free(q);
}
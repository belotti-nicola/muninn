#include <stdlib.h>
#include <stdio.h>
#include "internal/queue_message.h"

#include "internal/queue_rb.h"

queue_t queue_setup()
{
  queue_t q;
  q.last_index  = 0;
  q.first_index = 0;
  q.size        = 0;

  return q;
}

bool queue_pop(queue_t *q,queue_message_t *out)
{
  if(q->size == 0)
  {
    fprintf(stderr,"Popping empty queue.\n");
    return false;
  }

  int target_index  = q->first_index;
  *out              = q->buffer[target_index];

  target_index = (target_index + 1) % BUFFER_LENGTH;

  q->first_index = target_index;

  q->size--;

  return true;
}

bool queue_push(queue_t *q, queue_message_t *e)
{
  if(q->size >= BUFFER_LENGTH)
  {
    fprintf(stderr, "Limit reached. Size is:%d.\n", BUFFER_LENGTH);
    return false;
  }

  q->buffer[q->last_index] = *e;

  q->last_index = (q->last_index + 1) % BUFFER_LENGTH;

  q->size++;

  return true;
}
#include <stdlib.h>
#include <stdio.h>
#include "internal/queue_message.h"
#include <string.h>

#include "internal/queue_rb.h"

void queue_setup(queue_t *q,queue_message_t *buffer, size_t max_dim)
{
  q->last_index  = 0;
  q->first_index = 0;
  q->size        = 0;
  q->max_size    = max_dim;

  q->messages    = buffer;
}

bool queue_pop(queue_t *q,queue_message_t *out)
{
  if(q->size == 0)
  {
    fprintf(stderr,"Popping empty queue.\n");
    return false;
  }

  size_t offset  = q->first_index;
  queue_message_t *target = q->messages + offset;
  
  memcpy(out->data,target->data,target->size);
  out->data[target->size] = '\0';
  out->max_size = target->max_size;
  out->severity = target->severity;
  out->size = target->size;

  offset = (offset + 1) % q->max_size;

  q->first_index = offset;
  q->size--;

  return true;
}

bool queue_push(queue_t *q, log_severity_t severity, const char *s)
{
  if(q->size >= q->max_size)
  {
    fprintf(stderr, "Limit reached. Size is:%zu.\n", q->max_size);
    return false;
  }

  if (s == NULL) return false;

  size_t offset = q->last_index;
  queue_message_t *target = q->messages + offset;
  
  size_t len = strlen(s);

  memcpy(target->data, s, len);
  target->data[len] = '\0';
  target->size      = len;
  target->severity  = severity;
  
  q->last_index = (offset + 1) % q->max_size;
  q->size++;

  return true;
}
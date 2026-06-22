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

bool queue_pop(queue_t *q,uint8_t *out, size_t out_size, size_t *popped_bytes)
{
  if(q == NULL || out == NULL || popped_bytes == NULL || out_size == 0)
  {
    fprintf(stderr,"Error popping Queue.\n");
    return false;
  }

  size_t          offset  = q->first_index;
  queue_message_t *target = q->messages + offset;
  size_t              len = target->size;
  if( len > COMP_MESSAGE_SIZE )//TODO
  {
    fprintf(stderr, "Message size overflow in pop. Trucanting to %d bytes.\n", COMP_MESSAGE_SIZE);//TODO
    len = COMP_MESSAGE_SIZE;
  }

  memcpy(out,target->data,len);
  *popped_bytes = len;

  offset = (offset + 1) % q->max_size;

  q->first_index = offset;
  q->size--;

  return true;
}

bool queue_push(queue_t *q, const uint8_t *out, size_t out_size)
{
  if(q->size >= q->max_size)
  {
    fprintf(stderr, "Limit reached. Size is:%zu.\n", q->max_size);
    return false;
  }

  size_t offset           = q->last_index;
  queue_message_t *target = q->messages + offset;
  
  memcpy(target->data, out, out_size);
  target->data[out_size] = '\0';
  target->size           = out_size;
  
  q->last_index = (offset + 1) % q->max_size;
  q->size++;

  return true;
}
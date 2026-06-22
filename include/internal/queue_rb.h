#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "queue_message.h"

#include <internal/muninn_int.h>

typedef struct queue 
{
    size_t first_index;
    size_t last_index;
    size_t size;
    size_t max_size;

    queue_message_t *messages;

} queue_t;

void    queue_setup(queue_t *q,queue_message_t *buffer, size_t max_dim);
bool    queue_pop(queue_t *q, uint8_t *out, size_t out_size, size_t *bytes_popped);
bool    queue_push(queue_t *q, const uint8_t *buffer,size_t buffer_size);


#endif
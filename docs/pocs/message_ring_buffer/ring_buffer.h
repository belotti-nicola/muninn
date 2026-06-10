#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include "common_defs.h"
#include <stddef.h>

typedef struct ring_buffer
{
    char data[RB_DATA_SIZE];

    size_t cur_size;
    size_t start;
    size_t end;

} ring_buffer;

void rb_setup(ring_buffer *rb);
void rb_push(ring_buffer *rb,uint8_t severity,const char *message);
void rb_pop(ring_buffer *rb, message_t *msg);

#endif
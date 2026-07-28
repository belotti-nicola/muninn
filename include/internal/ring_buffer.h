#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct ring_buffer
{
    uint8_t *data;
    size_t   data_size;

    size_t start;
    size_t end;
    size_t current_size;

} ring_buffer;

bool   rb_setup(ring_buffer *rb, uint8_t *data, size_t data_size);
size_t rb_peek(ring_buffer *rb);
size_t rb_push(ring_buffer *rb, const uint8_t *raw_data, size_t length);
size_t rb_pop(ring_buffer *rb, uint8_t *out_buffer, size_t length);
size_t rb_available_space(const ring_buffer *rb);
#endif
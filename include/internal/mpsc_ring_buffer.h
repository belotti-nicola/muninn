#ifndef MPSC_RING_BUFFER_H
#define MPSC_RING_BUFFER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdatomic.h>

typedef struct mpsc_ring_buffer
{
    uint8_t *data;
    size_t   data_size;

    _Atomic size_t start;
    _Atomic size_t end;

} mpsc_ring_buffer;

bool mpsc_rb_setup(mpsc_ring_buffer *rb, uint8_t *data, size_t data_size);

// MULTIPLE PRODUCERS API
bool mpsc_rb_push(mpsc_ring_buffer *rb, uint8_t *data, size_t data_size);

// SINGLE CONSUMER APIs
bool mpsc_rb_peek(mpsc_ring_buffer *rb, uint8_t *data, size_t data_size);
bool mpsc_rb_advance(mpsc_ring_buffer *rb, size_t data_size);


#endif
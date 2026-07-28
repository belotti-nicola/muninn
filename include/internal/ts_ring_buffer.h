#ifndef THREADSAFE_RING_BUFFER_H
#define THREADSAFE_RING_BUFFER_H

#include <internal/ring_buffer.h>
#include <internal/ts_rb_message.h>
#include <internal/log_types.h>
#include <stdint.h>
#include "pthread.h"

typedef struct ts_ring_buffer 
{
    bool               stop;
    ring_buffer        ring_buffer;
    pthread_mutex_t    mutex;
    pthread_cond_t     empty;
    pthread_cond_t     full;

} ts_ring_buffer_t;

void         ts_rb_setup(ts_ring_buffer_t *tsrb,uint8_t *buffer, size_t buffer_dim);
bool         ts_rb_pop(ts_ring_buffer_t *tsrb, uint8_t *out, size_t out_size, size_t *bytes_popped);//TODO BYTES POPPED USELESS
bool         ts_rb_push(ts_ring_buffer_t* tsrb, const uint8_t *buffer, size_t buffer_size);
void         ts_rb_stop(ts_ring_buffer_t *tsrb);
void         ts_rb_release(ts_ring_buffer_t *tsrb);


#endif
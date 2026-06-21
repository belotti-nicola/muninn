#ifndef GATEWAY_TH_H
#define GATEWAY_TH_H

#include <stdatomic.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>

#include <internal/log_types.h>
#include <internal/ts_ring_buffer.h>
#include <internal/ts_queue.h>


typedef struct muninn_t muninn_t;

typedef struct gateway_th_data
{
    ts_ring_buffer_t *rb;

    ts_queue_t *q1;
    ts_queue_t *q2;

} gateway_th_data;

void *gateway_loop_fn(void *arg);
void *gateway_stop_fn(void *arg);
void *gateway_post_fn(void *context, void *data);


#endif
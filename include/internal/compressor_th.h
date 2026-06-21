#ifndef MUNINN_COMPRESSOR_H
#define MUNINN_COMPRESSOR_H

#include <internal/ts_queue.h>

typedef struct compressor_th_data
{
    ts_queue_t *q;

} compressor_th_data;

void *fcompressor_loop_fn(void *arg);
void *fcompressor_stop_fn(void *arg);
void *fcompressor_post_fn(void *context, void *arg);

#endif 
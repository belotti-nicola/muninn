#ifndef MUNINN_COMPRESSOR_H
#define MUNINN_COMPRESSOR_H

#include <stdatomic.h>
#include <pthread.h>
#include <internal/muninn_int.h>

#include <internal/ts_queue.h>

typedef struct compressor_th_data
{
    pthread_t    th;
    atomic_bool  running;
    ts_queue_t  *tasks;

} compressor_th_data;

int  compressor_th_start(compressor_th_data *cth_data);
int  compressor_th_stop(compressor_th_data *cth_data);
int  compressor_th_join(compressor_th_data *cth_data);
void compressor_th_perform(compressor_th_data *cth_data,const char *filepath);

static void *compressor_th_function(void *arg);

#endif 
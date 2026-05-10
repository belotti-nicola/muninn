#ifndef COMRPESSOR_TH_H
#define COMPRESSOR_TH_H

#include <pthread.h>
#include <internal/ts_queue.h>

typedef struct compressor_th_data
{
    pthread_t    th;
    ts_queue_t    q;

} compressor_th_data;



void *compressor_thread_function(void *arg);
void  compressor_thread_start(compressor_th_data *th);
void  compressor_thread_stop(compressor_th_data *th);

#endif
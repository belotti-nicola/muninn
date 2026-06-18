#ifndef LOGGER_TH_H
#define LOGGER_TH_H

#include <stdatomic.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>

#include <internal/ts_queue.h>
#include <internal/ts_ring_buffer.h>
#include <internal/console_handler.h>


typedef struct logger_th_data
{
    char         path[P_SIZE];
    pthread_t    th;
    atomic_bool  running;
    FILE*        file;
    uint64_t     written_bytes;

    ts_ring_buffer_t  *ringbuffer;
    ts_queue_t        *compress_q;

    console_handler_t  *console_handler;

} logger_th_data;

int  logger_th_start(void *arg);
int  logger_th_stop(void *arg);
int  logger_th_join(void *arg);
bool logger_th_perform(void *context,void *arg);

#endif
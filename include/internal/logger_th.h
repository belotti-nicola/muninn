#ifndef LOGGER_TH_H
#define LOGGER_TH_H

#include <stdatomic.h>
#include <pthread.h>
#include <stdint.h>

#include <internal/ts_queue.h>
#include <internal/ts_ring_buffer.h>


typedef struct logger_th_data
{
    char         path[P_SIZE];
    pthread_t    th;
    atomic_bool  running;
    FILE*        file;
    uint64_t     written_bytes;

    ts_ring_buffer_t  *ringbuffer;
    ts_queue_t        *compress_q;

} logger_th_data;

int  logger_th_start(logger_th_data *cth_data);
int  logger_th_stop(logger_th_data *cth_data);
int  logger_th_join(logger_th_data *cth_data);
void logger_th_perform(logger_th_data *cth_data, log_severity_t severity, const char *filepath);

static void *logger_th_function(void *arg);

#endif
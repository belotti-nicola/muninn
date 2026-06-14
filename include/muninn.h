#ifndef MUNINN_H
#define MUNINN_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdint.h>

#include <internal/munin_int.h>

#include <internal/compressor_th.h>
#include <internal/logger_th.h>

typedef struct {
    atomic_bool running;
    char path[P_SIZE];

    char               buffer_logger[LOG_RB_SIZE];
    char               buffer_compressor[COMP_MESSAGE_SIZE * COMP_QUEUE_SIZE];

    ts_rb_message_t    ringbuffer_logger;
    queue_message_t    queue_compressor[COMP_QUEUE_SIZE];
    
    ts_ring_buffer_t   logger_rb;
    ts_queue_t         compressor_q;

    logger_th_data     logger_th;
    compressor_th_data compressor_th;
    
} muninn_t;

void     muninn_init(muninn_t *m,const char *path);

void     muninn_log_dbg(muninn_t *muninn,const char *msg);
void     muninn_log_info(muninn_t *muninn,const char *msg);
void     muninn_log_warn(muninn_t *muninn,const char *msg);
void     muninn_log_error(muninn_t *muninn,const char *msg);
void     muninn_log_fatal(muninn_t *muninn,const char *msg);

void     muninn_shutdown(muninn_t *muninn);

#endif 
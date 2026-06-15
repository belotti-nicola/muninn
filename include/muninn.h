#ifndef MUNINN_H
#define MUNINN_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdint.h>

#include <internal/log_types.h>
#include <internal/munin_int.h>
#include <internal/compressor_th.h>
#include <internal/logger_th.h>
#include <internal/console_handler.h>

typedef struct muninn_t 
{
    uint64_t           start_ts;
    atomic_bool        running;
    atomic_char        threshold;//log level
    char               path[P_SIZE];

    char               buffer_logger[LOG_RB_SIZE];
    char               buffer_compressor[COMP_MESSAGE_SIZE * COMP_QUEUE_SIZE];

    ts_rb_message_t    ringbuffer_logger;
    queue_message_t    queue_compressor[COMP_QUEUE_SIZE];
    
    ts_ring_buffer_t   logger_rb;
    ts_queue_t         compressor_q;

    logger_th_data     logger_th;
    compressor_th_data compressor_th;

    console_handler_t   console_handler;
    
} muninn_t;

bool     muninn_init(muninn_t *m,const char *path);

         //DO NOT USE THIS
void     muninn_log_internal(muninn_t *m, log_severity_t severity, const char *file, int line, const char *fmt, ...);

         //USE THESE INSTEAD:
#define  muninn_log_info(m, fmt, ...)    muninn_log_internal(m, LOG_INFO,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define  muninn_log_debug(m, fmt, ...)   muninn_log_internal(m, LOG_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define  muninn_log_warning(m, fmt, ...) muninn_log_internal(m, LOG_WARN, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define  muninn_log_error(m, fmt, ...)   muninn_log_internal(m, LOG_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define  muninn_log_fatal(m, fmt, ...)   muninn_log_internal(m, LOG_FATAL, __FILE__, __LINE__, fmt, ##__VA_ARGS__)


void     muninn_shutdown(muninn_t *muninn);

void     muninn_set_dynamic_level(muninn_t *muninn, log_severity_t level);
void     muninn_panic_flush(muninn_t *muninn);

#endif 
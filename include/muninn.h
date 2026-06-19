#ifndef MUNINN_H
#define MUNINN_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdint.h>

#include <internal/log_types.h>
#include <internal/muninn_int.h>
#include <internal/compressor_th.h>
#include <internal/muninn_worker_th.h>
#include <internal/ts_ring_buffer.h>

#include <internal/gateway_th.h>
#include <internal/flogger_th.h>
#include <internal/clogger_th.h>



typedef struct muninn_t 
{
    uint64_t           start_ts;
    atomic_bool        running;
    atomic_char        threshold;//log level
    char               path[P_SIZE];

    char               buffer_compressor[COMP_MESSAGE_SIZE * COMP_QUEUE_SIZE];
    char               buffer_console[CONS_MESSAGE_SIZE * CONS_QUEUE_SIZE];

    queue_message_t    queue_console[CONS_QUEUE_SIZE];
    queue_message_t    queue_compressor[COMP_QUEUE_SIZE];
    
    ts_queue_t         console_q;
    ts_queue_t         compressor_q;

    //console_th_data    console_th;
    compressor_th_data compressor_th;
    gateway_th_data    gateway_th;
    flogger_th_data    flogger_th;

    char               gateway_buff[LOG_RB_SIZE];
    ts_ring_buffer_t   gateway_rb;
    ts_rb_message_t    gateway_message;
    muninn_worker_t    gateway;

    char               flogger_buff[LOG_QUEUE_SIZE * LOG_MESSAGE_SIZE];
    ts_queue_t         flogger_q;
    queue_message_t    flogger_m[LOG_MESSAGE_SIZE];
    muninn_worker_t    flogger;

    char               clogger_buff[LOG_QUEUE_SIZE * LOG_MESSAGE_SIZE];
    ts_queue_t         clogger_q;
    queue_message_t    clogger_m[LOG_MESSAGE_SIZE];
    muninn_worker_t    clogger;

    
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
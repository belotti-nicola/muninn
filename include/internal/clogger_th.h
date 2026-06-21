#ifndef CLOGGER_TH_H
#define CLOGGER_TH_H

#include <stdatomic.h>
#include <pthread.h>
#include <internal/muninn_int.h>
#include <internal/log_types.h>
#include <internal/ts_queue.h>
#include <internal/console_handler.h>

typedef struct muninn_t muninn_t;

typedef struct clogger_th_data
{
    console_handler_t ch;
    muninn_t *muninn;

} clogger_th_data;

void *clogger_loop_fn(void *arg);
void *clogger_stop_fn(void *arg);
void *clogger_join_fn(void *arg);
void *clogger_post_fn(void *context,void *arg);

#endif 
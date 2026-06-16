#ifndef MUNINN_CONSOLE_H
#define MUNINN_CONSOLE_H

#include <stdatomic.h>
#include <pthread.h>
#include <internal/muninn_int.h>
#include <internal/log_types.h>
#include <internal/ts_queue.h>
#include <internal/console_handler.h>


typedef struct console_th_data
{
    pthread_t          th;
    atomic_bool        running;
    ts_queue_t        *tasks;
    console_handler_t  ch;

} console_th_data;

int  console_th_start(console_th_data *cth_data);
int  console_th_stop(console_th_data *cth_data);
int  console_th_join(console_th_data *cth_data);
void console_th_perform(console_th_data *cth_data,log_severity_t severity, const char *message);

static void *console_th_function(void *arg);

#endif 
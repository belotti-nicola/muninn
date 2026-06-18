#ifndef LOGGER_TH_H
#define LOGGER_TH_H

#include <stdatomic.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include <internal/muninn_int.h>
#include <stdio.h>


typedef struct muninn_t muninn_t;

typedef struct flogger_th_data
{
    char         path[P_SIZE];
    FILE*        file;
    uint64_t     written_bytes;

    muninn_t    *muninn;

} flogger_th_data;

void *flogger_loop_fn(void *arg);
void *flogger_stop_fn(void *arg);
void *flogger_join_fn(void *arg);

void *flogger_post_fn(void *context, void *arg);

#endif
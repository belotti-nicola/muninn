#ifndef MUNINN_H
#define MUNINN_H


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdatomic.h>
#include "internal/ts_queue.h"

#include <internal/munin_int.h>

typedef struct {
    char path[P_SIZE];
    FILE* file;
    atomic_bool running;
    pthread_t thread;

    ts_queue_t q;

} muninn_t;

void     muninn_init(const char *path, muninn_t *m);
void     muninn_log(muninn_t *muninn,const char *msg);
void     muninn_shutdown(muninn_t *muninn);

#endif 
#ifndef MUNINN_H
#define MUNINN_H


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdatomic.h>
#include "internal/ts_queue.h"

#define PATH_LEN 256

typedef struct {
    char path[PATH_LEN];
    FILE* file;
    atomic_bool running;
    atomic_bool keep_looping;
    pthread_t thread;

    ts_queue_t q;

} muninn_t;

void     muninn_setup(const char *path, muninn_t *m);
void     muninn_start(muninn_t *muninn);
void     muninn_log(muninn_t *muninn,const char *msg);
void     muninn_join(muninn_t *muninn);


#endif 
#ifndef LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#include "log_queue.h"

typedef struct {
    char path[256];
    FILE* file;
    log_queue_t queue;
    bool running;
    pthread_t thread;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    size_t current_size;
    size_t rotate_size;

} muninn_t;

muninn_t muninn_create(const char *path, size_t max_size);
void     muninn_start(muninn_t muninn);
void     muninn_log(muninn_t muninn,const char *msg);
void     muninn_thread_function(void *arg);

#endif
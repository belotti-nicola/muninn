#ifndef MUNINN_WORKER_TH_H
#define MUNINN_WORKER_TH_H

#include <stdatomic.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct muninn_worker_t
{
    pthread_t          th;
    atomic_bool        running;
    const char        *name;

    void* (*thread_loop)   (void *arg);
    void* (*thread_stop)   (void *arg);
    void* (*thread_post)   (void *ctx, void *data, size_t data_size);
    void                         *context;

} muninn_worker_t;

int  mw_init(muninn_worker_t *muninn_worker, const char *name, 
    void* (*thread_loop)(void*),
    void* (*thread_stop)(void*),
    void* (*thread_perform)(void *arg,void *data, size_t data_size),
    void *context);
int  mw_start(muninn_worker_t *muninn_worker);
int  mw_stop(muninn_worker_t *muninn_worker);
int  mw_join(muninn_worker_t *muninn_worker);
int  mw_shutdown(muninn_worker_t *muninn_worker);
int  mw_post(muninn_worker_t *muninn_worker, void *data, size_t data_size);

bool mw_running(muninn_worker_t *muninn_worker);

#endif
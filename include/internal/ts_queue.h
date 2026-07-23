#ifndef THREADSAFE_QUEUE_H
#define THREADSAFE_QUEUE_H

#include "internal/queue_rb.h"
#include "pthread.h"

typedef struct ts_queue 
{
    bool               stop;
    queue_t            queue;
    pthread_mutex_t    mutex;
    pthread_cond_t     empty;
    pthread_cond_t     full;

} ts_queue_t;

void         ts_queue_setup(ts_queue_t *q,queue_message_t *buffer, size_t buffer_dim);
bool         ts_queue_pop(ts_queue_t* q, queue_message_t *out);
bool         ts_queue_push(ts_queue_t* q, log_severity_t severity, const char *message);
void         ts_queue_stop(ts_queue_t *q);
void         ts_queue_release(ts_queue_t *q);

bool         ts_queue_n_push(ts_queue_t* q, log_severity_t severity, const char *msg,size_t size);
bool         ts_queue_n_pop(ts_queue_t* q, uint8_t *out,size_t out_max_size, size_t *out_size);


#endif
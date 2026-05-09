#ifndef THREADSAFE_QUEUE_H
#define THREADSAFE_QUEUE_H

#include "internal/queue_rb.h"
#include "pthread.h"

typedef struct ts_queue 
{
    queue_t            queue;
    pthread_mutex_t    mutex;
    pthread_cond_t     empty;

} ts_queue_t;

ts_queue_t   ts_queue_setup();
bool         ts_queue_pop(ts_queue_t* q,queue_message_t *out);
bool         ts_queue_push(ts_queue_t* q,queue_message_t *msg);
void         ts_queue_release(ts_queue_t *q);

#endif
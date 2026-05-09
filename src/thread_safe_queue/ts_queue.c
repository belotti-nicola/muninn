
#include "ts_queue.h"

ts_queue_t ts_queue_setup()
{
    ts_queue_t retVal;

    retVal.queue = queue_setup();

    pthread_mutex_init(&retVal.mutex, NULL);
    pthread_cond_init(&retVal.empty, NULL);

    return retVal;
}

bool ts_queue_pop(ts_queue_t* q, queue_message_t *out)
{
    pthread_mutex_lock(&q->mutex);

    while (q->queue.size == 0)
    {
        pthread_cond_wait(
            &q->empty,
            &q->mutex
        );
    }
    bool retVal = queue_pop(&q->queue, out);

    pthread_mutex_unlock(&q->mutex);
    return retVal;
}

bool ts_queue_push(ts_queue_t* q, queue_message_t *msg)
{
    pthread_mutex_lock(&q->mutex);

    bool retVal = queue_push(&q->queue, msg);

    if(retVal && q->queue.size == 1)
    {
        pthread_cond_signal(&q->empty);
    }

    pthread_mutex_unlock(&q->mutex);

    return retVal;
}

void ts_queue_release(ts_queue_t *q)
{   
    pthread_mutex_destroy(
        &(q->mutex)
    );
     pthread_cond_destroy(
        &(q->empty)
    );
}
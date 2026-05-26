
#include "ts_queue.h"
#include <string.h>

void ts_queue_setup(ts_queue_t *tsq,queue_message_t *buffer, size_t buffer_dim)
{ 
    queue_setup(&tsq->queue,buffer,buffer_dim);
    tsq->stop = false;

    pthread_mutex_init(&tsq->mutex, NULL);
    pthread_cond_init(&tsq->empty, NULL);
}

bool ts_queue_pop(ts_queue_t* q,log_severity_t *severity, char *out)
{
    pthread_mutex_lock(&q->mutex);

    while (q->queue.size == 0 && !q->stop)
    {
        pthread_cond_wait(&q->empty, &q->mutex);
    }

    if (q->stop && q->queue.size == 0)
    {
        pthread_mutex_unlock(&q->mutex);
        return false;
    }

    bool retVal = queue_pop(&q->queue, severity, out);

    pthread_mutex_unlock(&q->mutex);
    return retVal;
}

bool ts_queue_push(ts_queue_t* q, log_severity_t severity, const char *msg)
{
    pthread_mutex_lock(&q->mutex);

    if (q->stop)
    {
        pthread_mutex_unlock(&q->mutex);
        return false;
    }

    bool retVal = queue_push(&q->queue,severity, msg);
    if  (retVal)
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

void ts_queue_stop(ts_queue_t *q)
{
    pthread_mutex_lock(&q->mutex);

    q->stop = true;
    pthread_cond_broadcast(&q->empty);
    
    pthread_mutex_unlock(&q->mutex);

}
#include <internal/muninn_worker.h>
#include "test_utils.h"
#include <internal/ts_queue.h>
#include <unistd.h>

#define BUFFER_SIZE 100
#define QUEUE_SIZE  5

typedef struct THREAD_CTX_T
{
    int        *counter;
    ts_queue_t *tsq;

} THREAD_CTX_T;

void *loop_fn(void *arg) 
{
    if(arg == NULL) return NULL;
    THREAD_CTX_T *ctx = (THREAD_CTX_T *)arg;

    if(ctx->tsq == NULL) return NULL;
    ts_queue_t *tsq = ctx->tsq;

    if(ctx->counter == NULL) return NULL;
    int *counter = ctx->counter;

    uint8_t payload_bytes[BUFFER_SIZE];
    size_t  payload_bytes_max_size = BUFFER_SIZE;

    queue_message_t msg;
    setup_queue_message(&msg,payload_bytes,payload_bytes_max_size);

    while(ts_queue_pop(tsq,&msg))
    {
        *counter = *counter + 1;
    }
    
    ts_queue_release(tsq);
    return NULL;
}

void *stop_fn(void *arg) 
{
    if(arg == NULL) return NULL;
    THREAD_CTX_T *ctx = (THREAD_CTX_T *)arg;

    ts_queue_t *tsq = ctx->tsq;
    ts_queue_stop(tsq);

    return NULL;
}

void *post_fn(void *arg,void *arg2) 
{
    if(arg == NULL) return NULL;
    THREAD_CTX_T *ctx = (THREAD_CTX_T *)arg;

    ts_queue_t *tsq = ctx->tsq;
    ts_queue_push(tsq,1,arg2);

    return NULL;
}


int main()
{  
    int offset = 0;
    char buffer[BUFFER_SIZE * QUEUE_SIZE]; 
    queue_message_t messages[QUEUE_SIZE] = {0};
    for(int i=0;i<QUEUE_SIZE;i++)
    {
        setup_queue_message(messages+i,buffer+offset,BUFFER_SIZE);
        offset += BUFFER_SIZE;
    }

    ts_queue_t tsq;
    ts_queue_setup(&tsq,messages,QUEUE_SIZE);

    int counter = 0;
    THREAD_CTX_T ctx = 
    {
        .counter = &counter,
        .tsq = &tsq
    };

    muninn_worker_t mw;
    mw_init(&mw,
        "test_function",
        loop_fn,
        stop_fn,
        post_fn,
        (void *)&ctx     
    );

    mw_post(&mw,"");
    mw_post(&mw,"");

    mw_shutdown(&mw);


    if( counter != 2) 
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: counter(%d) is not 2",counter);
    }
    return 0;
}
#include <internal/muninn_worker.h>
#include <internal/ts_ring_buffer.h>
#include <unistd.h>
#include <string.h>

#include "test_utils.h"

#define BUFFER_SIZE 1024

typedef struct THREAD_CTX_T
{
    int              *counter;
    ts_ring_buffer_t *tsrb;

} THREAD_CTX_T;

void *loop_fn(void *arg) 
{
    if(arg == NULL) return NULL;
    THREAD_CTX_T *ctx = (THREAD_CTX_T *)arg;

    uint8_t payload_bytes[BUFFER_SIZE];
    size_t  payload_bytes_max_size = BUFFER_SIZE;
    ts_ring_buffer_t *tsrb = ctx->tsrb;

    size_t popped_bytes = 0;
    while(ts_rb_pop(tsrb,payload_bytes,BUFFER_SIZE,&popped_bytes))
    {
        ctx->counter += 1;
    }
    
    return NULL;
}

void *stop_fn(void *arg) 
{
    if(arg == NULL) return NULL;
    THREAD_CTX_T *ctx = (THREAD_CTX_T *)arg;

    ts_ring_buffer_t *tsrb = ctx->tsrb;
    ts_rb_stop(tsrb);

    return NULL;
}

void *post_fn(void *arg,void *arg2, size_t arg3) 
{
    if(arg == NULL) return NULL;
    THREAD_CTX_T *ctx = (THREAD_CTX_T *)arg;

    ts_ring_buffer_t *tsrb = ctx->tsrb;
    const char *target = (const char *)arg2;
    ts_rb_push(tsrb,target,arg3);

    return NULL;
}

static void *context = NULL;

int main()
{  
    int counter = 0;

    uint8_t buffer[BUFFER_SIZE];

    ts_ring_buffer_t tsrb;
    ts_rb_setup(&tsrb,buffer,BUFFER_SIZE);

    THREAD_CTX_T ctx = 
    {
        .counter = &counter,
        .tsrb = &tsrb
    };

    muninn_worker_t mw;
    mw_init(&mw,
        "test_function",
        loop_fn,
        stop_fn,
        post_fn,
        context     
    );

    mw_post(&mw,"1",1);
    mw_post(&mw,"1",1);

    mw_shutdown(&mw);


    if( counter != 2) 
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: counter(%d) is not 2",counter);
    }
    
    return 0;
}
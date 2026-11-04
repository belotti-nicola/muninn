#include <internal/muninn_worker.h>
#include <internal/gateway_th.h>
#include <internal/ts_ring_buffer.h>
#include <string.h>

#include <internal/muninn_message.h>

#include "test_utils.h"

#define BUFFER_SIZE 100
#define QUEUE_SIZE  5

int main()
{
    //RINGBUFFER
    ts_ring_buffer_t tsrb;
    uint8_t buffer[BUFFER_SIZE];
    ts_rb_setup(&tsrb,buffer,BUFFER_SIZE);

    int offset;

    //FIRST QUEUE
    offset = 0;
    ts_queue_t q1;
    char buffer1[BUFFER_SIZE * QUEUE_SIZE]; 
    queue_message_t messages1[QUEUE_SIZE] = {0};
    for(int i=0;i<QUEUE_SIZE;i++)
    {
        setup_queue_message(messages1+i,buffer1+offset,BUFFER_SIZE);
        offset += BUFFER_SIZE;
    }
    ts_queue_setup(&q1,messages1,QUEUE_SIZE);


    //SECOND QUEUE
    offset = 0;
    ts_queue_t q2;
    char buffer2[BUFFER_SIZE * QUEUE_SIZE]; 
    queue_message_t messages2[QUEUE_SIZE] = {0};
    for(int i=0;i<QUEUE_SIZE;i++)
    {
        setup_queue_message(messages2+i,buffer2+offset,BUFFER_SIZE);
        offset += BUFFER_SIZE;
    }
    ts_queue_setup(&q2,messages2,QUEUE_SIZE);

    muninn_encoding_mask_t mem = {0};
    muninn_encoding_mask_init(&mem,MEDM_MESSAGE);

    gateway_th_data gwdata = {0};
    gwdata.rb   = &tsrb;
    gwdata.q1   = &q1;
    gwdata.q2   = &q2;
    gwdata.mask = &mem;

    muninn_worker_t mw;
    mw_init(&mw,
        "test_function",
        gateway_loop_fn,
        gateway_stop_fn,
        gateway_post_fn,
        (void *)&gwdata     
    );
    mw_start(&mw);

    uint8_t m1[] = {'0','0','H','e','l','l','o'};
    size_t  m1_message_size = sizeof(m1) - 2;
    uint8_t expected_m1_encoding[] = "80Hello";
    uint8_t m2[] = {'0','0','W','o','r','l','d','!'};
    size_t  m2_message_size = sizeof(m2) - 2;
    uint8_t expected_m2_encoding[] = "80World!";

    mw_post(&mw,m1,sizeof(m1));
    mw_post(&mw,m2,sizeof(m2));

    sleep_ms(10);

    mw_shutdown(&mw);

    uint8_t computed[1024] = {0};
    size_t  computed_size;

    if(q1.queue.size != 2)
    {
        TRACE_ERROR_POSITION("Q1 size is %ld instead of 2",q1.queue.size);
        return 1;
    }

    if ( queue_pop(&q1.queue,computed,1024,&computed_size) == false )
    {
        TRACE_ERROR_POSITION("first pop failed");
        return 1;
    }

    if ( computed_size != m1_message_size)
    {
        TRACE_ERROR_POSITION("first pop failed: incorrect popped size (computed %ld instead of %ld)",computed_size,m1_message_size);
        return 1;
    }

    for ( int i = 0 ; i < computed_size ; i++ )
    {
        if ( computed[ i ] != expected_m1_encoding[ i + 2 ] ) 
        {
            TRACE_ERROR_POSITION("check data fail: found %d instead of %d(iteration %d)",computed[ i ], expected_m1_encoding[ i + 2 ],i);
            return 1;
        }
    }

    if ( queue_pop(&q1.queue,computed,1024,&computed_size) == false )
    {
        TRACE_ERROR_POSITION("second pop failed");
        return 1;
    }

    if ( computed_size != m2_message_size)
    {
        TRACE_ERROR_POSITION("second pop failed: incorrect popped size (computed %ld instead of %ld)",computed_size,m2_message_size);
        return 1;
    }

    for ( int i = 0 ; i < computed_size ; i++ )
    {
        if ( computed[ i ] != expected_m2_encoding[ i + 2 ] ) 
        {
            TRACE_ERROR_POSITION("check data fail: found %d instead of %d(iteration %d)",computed[ i ], expected_m2_encoding[ i + 2 ],i);
            return 1;
        }
    }


    if(q2.queue.size != 2)
    {
        TRACE_ERROR_POSITION("Q2 size is %ld instead of 2",q2.queue.size);
        return 1;
    }

    if ( queue_pop(&q2.queue,computed,1024,&computed_size) == false )
    {
        TRACE_ERROR_POSITION("first pop failed");
        return 1;
    }

    if ( computed_size != m1_message_size)
    {
        TRACE_ERROR_POSITION("first pop failed: incorrect popped size (computed %ld instead of %ld)",computed_size,m1_message_size);
        return 1;
    }

    for ( int i = 0 ; i < computed_size ; i++ )
    {
        if ( computed[ i ] != expected_m1_encoding[ i + 2 ] ) 
        {
            TRACE_ERROR_POSITION("check data fail: found %d instead of %d(iteration %d)",computed[ i ], expected_m1_encoding[ i + 2 ],i);
            return 1;
        }
    }

    if ( queue_pop(&q2.queue,computed,1024,&computed_size) == false )
    {
        TRACE_ERROR_POSITION("second pop failed");
        return 1;
    }

    if ( computed_size != m2_message_size)
    {
        TRACE_ERROR_POSITION("second pop failed: incorrect popped size (computed %ld instead of %ld)",computed_size,m2_message_size);
        return 1;
    }

    for ( int i = 0 ; i < computed_size ; i++ )
    {
        if ( computed[ i ] != expected_m2_encoding[ i + 2 ] ) 
        {
            TRACE_ERROR_POSITION("check data fail: found %d instead of %d(iteration %d)",computed[ i ], expected_m2_encoding[ i + 2 ],i);
            return 1;
        }
    }


    return 0;
}
#include <internal/muninn_worker.h>
#include <internal/gateway_th.h>
#include <internal/ts_ring_buffer.h>

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

    gateway_th_data gwdata = {0};
    gwdata.rb = &tsrb;
    gwdata.q1 = &q1;
    gwdata.q2 = &q2;

    muninn_worker_t mw;
    mw_init(&mw,
        "test_function",
        gateway_loop_fn,
        gateway_stop_fn,
        gateway_post_fn,
        (void *)&gwdata     
    );
    mw_start(&mw);

    mw_post(&mw,"Hello");
    mw_post(&mw,"World");

    mw_shutdown(&mw);

    sleep_ms(10);

    // if(q1.queue.size != 2)
    // {
    //     TRACE_ERROR_POSITION();
    //     TEST_ERROR("Q1 size is %ld instead of 2",q1.queue.size);
    //     return 1;
    // }

    // if(q2.queue.size != 2)
    // {
    //     TRACE_ERROR_POSITION();
    //     TEST_ERROR("Q2 size is %ld instead of 2",q2.queue.size);
    //     return 1;
    // }

    return 0;
}
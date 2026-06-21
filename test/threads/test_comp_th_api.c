#include <internal/muninn_worker.h>
#include <internal/compressor_th.h>
#include <internal/ts_ring_buffer.h>
#include "test_utils.h"

#define MESSAGE_SIZE 100
#define QUEUE_SIZE 5

int main()
{
    int offset = 0;
    char buffer[MESSAGE_SIZE * QUEUE_SIZE];
    queue_message_t messages[QUEUE_SIZE] = {0};
    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        setup_queue_message(messages + i, buffer + offset, MESSAGE_SIZE);
        offset += MESSAGE_SIZE;
    }

    ts_queue_t tsq;
    ts_queue_setup(&tsq, messages, QUEUE_SIZE);

    compressor_th_data data = {0};
    data.q = &tsq;

    muninn_worker_t mw = {0};
    mw_init(&mw,"test_worker",
        fcompressor_loop_fn,
        fcompressor_stop_fn,
        fcompressor_post_fn,
        (void *)&data
    );
    mw_start(&mw);
    
    sleep_ms(10);
    if(mw_running(&mw) == false)
    {
        mw_shutdown(&mw);
        TRACE_ERROR_POSITION();
        TEST_ERROR("Running boolean is false instead of true during the test.");
        return 1;
    }

    mw_shutdown(&mw);

    sleep_ms(10);
    if(mw_running(&mw) == true)
    {
        mw_shutdown(&mw);
        TRACE_ERROR_POSITION();
        TEST_ERROR("Running boolean is true instead of false at the end of the test.");
        return 1;
    }


    return 0;
}
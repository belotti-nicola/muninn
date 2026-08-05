#include <internal/muninn_worker.h>
#include <internal/gateway_th.h>
#include <internal/ts_ring_buffer.h>

#include "test_utils.h"

#define BUFFER_SIZE 100

int main()
{
    ts_ring_buffer_t tsrb;
    uint8_t buffer[BUFFER_SIZE];
    ts_rb_setup(&tsrb,buffer,BUFFER_SIZE);

    gateway_th_data gwdata = {0};
    gwdata.rb = &tsrb;
    gwdata.q1 = NULL;
    gwdata.q2 = NULL;

    muninn_worker_t mw;
    mw_init(&mw,
        "test_function",
        gateway_loop_fn,
        gateway_stop_fn,
        gateway_post_fn,
        (void *)&gwdata     
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

    if(mw_running(&mw) == true)
    {
        mw_shutdown(&mw);
        TRACE_ERROR_POSITION();
        TEST_ERROR("Running boolean is true instead of false at the end of the test.");
        return 1;
    }

    return 0;
}
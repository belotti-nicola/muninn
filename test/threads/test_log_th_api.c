#include <internal/muninn_worker.h>
#include <internal/flogger_th.h>
#include <internal/ts_queue.h>
#include <string.h>

#include "test_utils.h"

#define PATH_SIZE   100
#define BUFFER_SIZE 100
#define QUEUE_SIZE  5

int main()
{
    int offset = 0;
    ts_queue_t q;
    char buffer1[BUFFER_SIZE * QUEUE_SIZE]; 
    queue_message_t message[QUEUE_SIZE] = {0};
    for(int i=0;i<QUEUE_SIZE;i++)
    {
        setup_queue_message(message+i,buffer1+offset,BUFFER_SIZE);
        offset += BUFFER_SIZE;
    }
    ts_queue_setup(&q,message,QUEUE_SIZE);

    char test_log[PATH_SIZE];
    if(compute_test_file_name(test_log,PATH_SIZE) == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Could not compute test file name in %d bytes.",PATH_SIZE);
        return 1;
    }

    flogger_th_data flogger_data;
    strcpy(flogger_data.path,test_log);
    flogger_data.reading_queue = &q;

    muninn_worker_t mw;
    mw_init(&mw,
        "test_function",
        flogger_loop_fn,
        flogger_stop_fn,
        flogger_post_fn,
        (void *)&flogger_data     
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
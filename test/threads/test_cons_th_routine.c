#include <internal/muninn_worker.h>
#include <internal/clogger_th.h>
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


    clogger_th_data clogger_data;
    clogger_data.q = &q;

    muninn_worker_t mw;
    mw_init(&mw,
        "test_function",
        clogger_loop_fn,
        clogger_stop_fn,
        clogger_post_fn,
        (void *)&clogger_data     
    );
    mw_start(&mw);
    
    mw_post(&mw,"Hello");
    mw_post(&mw,"World");
    

    mw_shutdown(&mw);  

    return 0;
}
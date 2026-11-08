#include <internal/muninn_worker.h>
#include <internal/clogger_th.h>
#include <internal/ts_queue.h>
#include <string.h>

#include <internal/protocols/muninn_encoded_messages/muninn_encoded_message_codec.h>
#include <internal/muninn_message.h>

#include "test_utils.h"

#define PATH_SIZE   100
#define BUFFER_SIZE 100
#define QUEUE_SIZE  5
#define ENCODED_BUFFER_SIZE 512

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
    clogger_data.ch.is_terminal = true;

    muninn_worker_t mw;
    mw_init(&mw,
        "test_function",
        clogger_loop_fn,
        clogger_stop_fn,
        clogger_post_fn,
        (void *)&clogger_data     
    );
    mw_start(&mw);

    muninn_message_t mm1;
    mm1.severity = 255;
    mm1.msg  = "Hello";
    mm1.msg_len = strlen("Hello");

    muninn_message_t mm2;
    mm2.severity = 255;
    mm2.msg  = "World";
    mm2.msg_len = strlen("World");

    uint8_t first[ENCODED_BUFFER_SIZE] = {0};
    size_t first_size = 0;
    if( muninn_messages_encode(&mm1,MEDM_MESSAGE,first,ENCODED_BUFFER_SIZE,&first_size) == false )
    {
        TEST_ERROR("It was not possible to encode the first message");
        return 1;
    }
    uint8_t second[ENCODED_BUFFER_SIZE] = {0};
    size_t second_size = 0;
    if( muninn_messages_encode(&mm2,MEDM_MESSAGE,second,ENCODED_BUFFER_SIZE,&second_size) == false )
    {
        TEST_ERROR("It was not possible to encode the second message");
        return 1;
    }

    mw_post(&mw,first,first_size);
    mw_post(&mw,second,second_size);
    
    mw_shutdown(&mw);  

    TEST_INFO("Success");
    return 0;
}
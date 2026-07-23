#include <internal/muninn_worker.h>
#include <internal/gateway_th.h>
#include <internal/ts_ring_buffer.h>
#include <string.h>
#include <stdlib.h>

#include "test_utils.h"


#define MESSAGE_SIZE            120
#define MESSAGE_PER_THREAD    80000
#define THREADS_NUMBER           30

#define TEST_QUEUE_SIZE   (MESSAGE_PER_THREAD * THREADS_NUMBER + 100)
#define TEST_BUFFER_SIZE  (MESSAGE_SIZE + 50)

void* stress_producer_routine(void *arg)
{
    muninn_worker_t *mw = (muninn_worker_t *)arg;
    
    char message[MESSAGE_SIZE];
    memset(message,(int)'a',MESSAGE_SIZE);
    message[MESSAGE_SIZE-1] ='\0';

    for (int i = 0; i < MESSAGE_PER_THREAD; i++)
    {
        mw_post(mw,message,MESSAGE_SIZE);
    }

    return NULL;
}

int main()
{
    char *buffer1 = malloc(sizeof(char) * TEST_BUFFER_SIZE * TEST_QUEUE_SIZE);
    if(buffer1 == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Malloc fail.");
        return 1;
    }
    char *buffer2 = malloc(sizeof(char) * TEST_BUFFER_SIZE * TEST_QUEUE_SIZE);
    if(buffer2 == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Malloc fail.");
        return 1;
    }
    queue_message_t *messages1 = calloc(TEST_QUEUE_SIZE, sizeof(queue_message_t));
    if(messages1 == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Malloc fail.");
        return 1;
    }
    queue_message_t *messages2 = calloc(TEST_QUEUE_SIZE, sizeof(queue_message_t));
    if(messages2 == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Malloc fail.");
        return 1;
    }

    //RINGBUFFER
    ts_ring_buffer_t tsrb = {0};
    uint8_t buffer[LOG_RB_SIZE];
    ts_rb_setup(&tsrb,buffer,LOG_RB_SIZE);

    int offset;

    //FIRST QUEUE
    offset = 0;
    ts_queue_t q1;
    for(int i=0;i<TEST_QUEUE_SIZE;i++)
    {
        setup_queue_message(messages1+i,buffer1+offset,TEST_BUFFER_SIZE);
        offset += TEST_BUFFER_SIZE;
    }
    ts_queue_setup(&q1,messages1,TEST_QUEUE_SIZE);

    //SECOND QUEUE
    offset = 0;
    ts_queue_t q2;
    for(int i=0;i<TEST_QUEUE_SIZE;i++)
    {
        setup_queue_message(messages2+i,buffer2+offset,TEST_BUFFER_SIZE);
        offset += TEST_BUFFER_SIZE;
    }
    ts_queue_setup(&q2,messages2,TEST_QUEUE_SIZE);

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

    sleep_ms(100);
    
    pthread_t competitors[THREADS_NUMBER];
    for (int i = 0; i < THREADS_NUMBER; i++)
    {
        pthread_create(&competitors[i], NULL, stress_producer_routine, &mw);
    }

    for(int i = 0; i < THREADS_NUMBER; i++)
    {
        pthread_join(competitors[i],NULL);
    }

    sleep_ms(1000*10);
    mw_shutdown(&mw);


    if(q1.queue.size < THREADS_NUMBER * MESSAGE_PER_THREAD)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("%ld vs %d",q1.queue.size,THREADS_NUMBER * MESSAGE_PER_THREAD);
        free(buffer1);
        free(buffer2);
        free(messages1);
        free(messages2);
        return 1;
    }


    free(buffer1);
    free(buffer2);
    free(messages1);
    free(messages2);

    return 0;
}
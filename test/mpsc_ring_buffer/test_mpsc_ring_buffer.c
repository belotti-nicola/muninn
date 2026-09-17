#include "test_utils.h"

#include <internal/mpsc_ring_buffer.h>
#include <semaphore.h>
#include <pthread.h>

#define BUFFER_SIZE 1024
#define PRODUCERS 8
#define MESSAGES_PER_THREAD 10

#define MESSAGE_LEN 10

typedef struct threads_data
{
    mpsc_ring_buffer *rb;
    sem_t            *sem;

} threads_data;

void *consumer_fun(void *arg)
{
    threads_data *data = (threads_data *)arg;

    uint8_t peek_buffer[1] = {0};
    uint8_t advance_buffer[100] = {0};

    int expected_messages = MESSAGES_PER_THREAD * PRODUCERS;
    while(expected_messages != 0)
    {
        sem_wait(data->sem);

        mpsc_rb_peek(data->rb,peek_buffer,1);
        mpsc_rb_peek(data->rb,advance_buffer,MESSAGE_LEN);
        mpsc_rb_advance(data->rb,MESSAGE_LEN);

        expected_messages--;

    }

    return NULL;
}

void *producers_fun(void *arg)
{
    threads_data *data = (threads_data *)arg;

    const uint8_t *message = (const uint8_t*)"0123456789";

    int messages_counter = 0;
    while(messages_counter < MESSAGES_PER_THREAD)
    {
        mpsc_rb_push(data->rb,message,MESSAGE_LEN);
        sem_post(data->sem);
        messages_counter++;
    }

    return NULL;
}

int main()
{
    sem_t semaphore;
    sem_init(&semaphore,0,0);

    uint8_t buffer[BUFFER_SIZE] = {0};
    mpsc_ring_buffer rb = {0};
    if ( mpsc_rb_setup(&rb,buffer,BUFFER_SIZE) == false)
    {
        TRACE_ERROR_POSITION();
        TEST_INFO("Error: setup-ing the ring buffer");
        return false;
    }

    threads_data data = 
    {
        .rb = &rb,
        .sem = &semaphore
    };


    pthread_t consumer;
    pthread_create(&consumer,NULL,consumer_fun,&data);
    
    pthread_t producer[PRODUCERS];
    for(int i=0;i<PRODUCERS;i++)
    {
        pthread_create(&producer[i],NULL,producers_fun,&data);
    }

    sleep_ms(50);

    pthread_join(consumer,NULL);
    for(int i=0;i<PRODUCERS;i++)
    {
        pthread_join(producer[i],NULL);
    }

    sem_destroy(&semaphore);

    if(rb.start != rb.end)
    {
        TRACE_ERROR_POSITION();
        TEST_INFO("Error: ring buffer is not empty!");
        return 1;
    }
    
    return 0;
}
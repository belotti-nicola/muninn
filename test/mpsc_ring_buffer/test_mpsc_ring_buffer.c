#include <internal/mpsc_ring_buffer.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>

#include "test_utils.h"

#define BUFFER_SIZE   2*1024*1024  
#define THREADS                 8
#define MESSAGES            20000
#define MESSAGES_LEN           20

void *consumer_function(void *arg)
{
    bool peek_exit_code;
    bool advance_exit_code;

    mpsc_ring_buffer *sb = (mpsc_ring_buffer *)arg;

    uint8_t buff[MESSAGES_LEN] = {0};

    for (int i = 0; i< MESSAGES * THREADS ; i++)
    {
        peek_exit_code = mpsc_rb_peek(sb,buff,MESSAGES_LEN);
        if ( peek_exit_code == false )
        {
            perror("mpsc_rb_peek fail");
            exit(-1);
        }

        advance_exit_code = mpsc_rb_advance(sb,MESSAGES_LEN);
        if ( advance_exit_code == false )
        {
            perror("mpsc_rb_peek fail");
            exit(-1);
        }
    }

    return NULL;
}

void *producers_function(void *arg)
{
    bool push_exit_code;

    mpsc_ring_buffer *sb = (mpsc_ring_buffer *)arg;

    uint8_t buff[MESSAGES_LEN] = {0};
    for (int i = 0; i< MESSAGES ; i++)
    {
        push_exit_code = mpsc_rb_push(sb,buff,MESSAGES_LEN);
        if ( push_exit_code == false )
        {
            perror("producers_function fail");
            exit(-1);
        }
    }

    return NULL;
}

int main()
{
    bool exit_code;
    uint8_t buffer[BUFFER_SIZE] = {0};    
        
    mpsc_ring_buffer sb = {0};
    exit_code = mpsc_rb_init(&sb,buffer, BUFFER_SIZE);
    if (exit_code == false)
    {
        TEST_ERROR("sp_mpsc_sb_init failed");
        return 1;
    }

    pthread_t consumer;
    if ( pthread_create(&consumer,NULL,consumer_function,&sb) != 0 )
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Could not create consumer.");
        return 1;
    }

    pthread_t producers[THREADS];
    for( int i=0 ; i< THREADS ; i++)
    {
        if ( pthread_create(&producers[i],NULL,producers_function,&sb) != 0)
        {
            TRACE_ERROR_POSITION();
            TEST_ERROR("Could not create producer.");
            return 1;
        }
    }

    for( int i=0 ; i< THREADS ; i++)
    {
        pthread_t producer = producers[i];
        pthread_join(producer,NULL);
    }

    pthread_join(consumer,NULL);

    return 0;
}


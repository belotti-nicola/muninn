#include <internal/muninn_worker.h>
#include <internal/flogger_th.h>
#include <internal/ts_queue.h>
#include <string.h>
#include <stdlib.h>

#include "test_utils.h"

#define PATH_SIZE               256

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
        mw_post(mw,message);
    }

    return NULL;
}


int main()
{
    char filepath[PATH_SIZE];
    if(compute_test_file_name(filepath,PATH_SIZE) == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Could not compute test file name in %d bytes.",PATH_SIZE);
        return 1;
    }

    char buffer[LOG_MESSAGE_SIZE * LOG_QUEUE_SIZE] = {0};

    size_t offset = 0;
    queue_message_t messages[LOG_QUEUE_SIZE] = {0};
    for(int i=0;i<LOG_QUEUE_SIZE;i++)
    {
        setup_queue_message(messages+i,buffer+offset,LOG_MESSAGE_SIZE);
        offset += LOG_MESSAGE_SIZE;
    }
    ts_queue_t q;
    ts_queue_setup(&q,messages,LOG_QUEUE_SIZE);

    flogger_th_data flogger_data = {0};
    strcpy(flogger_data.path,filepath);
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
    
    sleep_ms(100);
    
    pthread_t competitors[THREADS_NUMBER];
    for(int i=0;i<THREADS_NUMBER;i++)
    {
        pthread_create(competitors+i,NULL,stress_producer_routine,(void *)&mw);
    }

    for(int i=0;i<THREADS_NUMBER;i++)
    {
        pthread_join(competitors[i],NULL);
    }

    mw_shutdown(&mw);

    char cwd[PATH_SIZE];
    if(get_executable_directory(cwd,PATH_SIZE) == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: could not detect executable directory.");
        return 1;
    }

    long counter = count_rows_across_files(cwd,"test_log_th_stress");
    if( counter != THREADS_NUMBER * MESSAGE_PER_THREAD)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("%ld vs %d",counter, THREADS_NUMBER * MESSAGE_PER_THREAD);
        return 1;
    }
    
    return 0;
}
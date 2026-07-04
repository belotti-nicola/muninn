#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <muninn.h>
#include <string.h>
#include <dirent.h>


#include "test_utils.h"

#define PATHSIZE 200

#define NUM_THREADS         40
#define MSG_PER_THREAD   50000

#define MESSAGE_SIZE       400

void* stress_producer_routine(void *arg)
{
    muninn_t *m = (muninn_t *)arg;
    char msg_buffer[MESSAGE_SIZE];
    memset(msg_buffer,(int)'a',MESSAGE_SIZE);
    msg_buffer[MESSAGE_SIZE-1] ='\0';

    for (int i = 0; i < MSG_PER_THREAD; i++)
    {
        muninn_log_info(m,msg_buffer);
    }

    return NULL;
}

int main(void)
{
    char testlog[PATHSIZE];
    if(compute_test_file_name(testlog,PATHSIZE) == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Could not compute test file.");
    }

    CONFIG CONF;
    muninn_config_default(&CONF);
    muninn_config_set_file(&CONF,true,testlog);
    muninn_config_set_console(&CONF,false,false);
    muninn_config_set_compressor(&CONF,false);

    muninn_t muninn;
    muninn_init(&muninn,CONF);

    pthread_t competitors[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&competitors[i], NULL, stress_producer_routine, &muninn);
    }

    for(int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(competitors[i],NULL);
    }

    muninn_shutdown(&muninn);

    char cwd[PATHSIZE];
    if(get_executable_directory(cwd,PATHSIZE) == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: could not detect executable directory.");
        return 1;
    }
    if(count_rows_across_files(cwd,"test_stressing") < MSG_PER_THREAD * NUM_THREADS * 0.99 )
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("countRows fail:%ld instead of %f.",count_rows_across_files(cwd,"test_stressing"),NUM_THREADS * MSG_PER_THREAD * 0.99);
        return 1;
    }

    return 0;
}
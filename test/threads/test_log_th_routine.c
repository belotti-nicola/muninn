#include <internal/muninn_worker.h>
#include <internal/flogger_th.h>
#include <internal/ts_queue.h>
#include <string.h>

#include <internal/ts_rb_encoder.h>

#include "test_utils.h"

#define PATH_SIZE   100
#define BUFFER_SIZE 100
#define QUEUE_SIZE  5

int main()
{
    int offset = 0;
    ts_queue_t q;
    char buffer[BUFFER_SIZE * QUEUE_SIZE]; 
    queue_message_t message[QUEUE_SIZE] = {0};
    for(int i=0;i<QUEUE_SIZE;i++)
    {
        setup_queue_message(message+i,buffer+offset,BUFFER_SIZE);
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

    flogger_th_data flogger_data = {0};
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
    
    
    char first[] = "TestLog\n";
    mw_post(&mw,first, strlen(first));
    char second[] = "ThRoutine\n";
    mw_post(&mw,second,strlen(second));

    sleep_ms(10);   
    mw_shutdown(&mw);

    FILE *test_file = fopen(test_log, "r");
    if(test_file == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: char pointer is null for file %s.",test_log);
        return 1;
    }

    char tmp[256] = {0};
    
    if(fgets(tmp, 256, test_file) == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("%s",test_log);
        TEST_ERROR("Error: char pointer is null.");
        return 1;
    }
    if(strstr(tmp, "TestLog") == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("%s",test_log);
        TEST_ERROR("Error: \"%s\" is not substring of \"%s\".", "TestLog", tmp);
        return 1;
    }

    if(fgets(tmp, 256, test_file) == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("%s",test_log);
        TEST_ERROR("Error: char pointer is null.");
        return 1;
    }
    if(strstr(tmp, "ThRoutine") == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("%s",test_log);
        TEST_ERROR("Error: \"%s\" is not substring of \"%s\".", "ThRoutine", tmp);
        return 1;
    }

    if (fgets(tmp, sizeof(tmp), test_file) != NULL)
    {
        TEST_ERROR("Unexpected extra line: %s", tmp);
        return 1;
    }

    if (ferror(test_file))
    {
        TEST_ERROR("Read error");
        return 1;
    }

    fclose(test_file);
    return 0;
}
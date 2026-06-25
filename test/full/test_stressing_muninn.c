#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <muninn.h>
#include <string.h>

#include "test_utils.h"

#define PATHSIZE 200

#define NUM_THREADS       20
#define MSG_PER_THREAD  1000

#define MESSAGE_SIZE    128

void* stress_producer_routine(void *arg)
{
    muninn_t *m = (muninn_t *)arg;
    char msg_buffer[MESSAGE_SIZE];
    memset(msg_buffer,(int)'a',127);
    msg_buffer[MESSAGE_SIZE-1] ='\0';

    for (int i = 0; i < MSG_PER_THREAD; i++)
    {
        muninn_log_info(m,msg_buffer);
        sleep_ms(50);
    }

    return NULL;
}

long countRows(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return -1;  // errore apertura file
    }

    long rows = 0;
    int c;

    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            rows++;
        }
    }

    fclose(fp);
    return rows;
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

    muninn_t muninn;
    muninn_init(&muninn,CONF);

    pthread_t competitors[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&competitors[i], NULL, stress_producer_routine, &muninn);
    }

    muninn_shutdown(&muninn);

    if(countRows(testlog) < NUM_THREADS * MSG_PER_THREAD * 0.99 )
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("countRows fail:%ld instead of %d.",countRows(testlog),NUM_THREADS * MSG_PER_THREAD);
        return 1;
    }

    return 0;
}
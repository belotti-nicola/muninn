#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <muninn.h>
#include <string.h>

#include "test_utils.h"

#define PATHSIZE 200

#define NUM_THREADS    10
#define MSG_PER_THREAD 10

void* stress_producer_routine(void *arg)
{
    muninn_t *m = (muninn_t *)arg;
    char msg_buffer[128];
    memset(msg_buffer,(int)'a',127);
    msg_buffer[127] ='\0';

    for (int i = 0; i < MSG_PER_THREAD; i++)
    {
        muninn_log_info(m,msg_buffer);
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

    muninn_t m ={0};
    muninn_init(&m,testlog);

    pthread_t competitors[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&competitors[i], NULL, stress_producer_routine, &m);
    }

    muninn_shutdown(&m);

    if(countRows(testlog) < NUM_THREADS * MSG_PER_THREAD * 0.5 )
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("countRows fail:%ld instead of %d.",countRows(testlog),NUM_THREADS * MSG_PER_THREAD);
    }

    return 1;
}
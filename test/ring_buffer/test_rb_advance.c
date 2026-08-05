#include <internal/ring_buffer.h>
#include <stdio.h>
#include <string.h>

#include "test_utils.h"


#define BUFFER_SIZE 100
#define TEST_STRING "testing_rb_peek"


int  main()
{
    uint8_t buffer[BUFFER_SIZE] = {0};
    size_t test_size = strlen(TEST_STRING);
    memcpy(buffer,TEST_STRING,test_size);


    ring_buffer rb = {0};
    if (rb_setup(&rb,buffer,BUFFER_SIZE) == false)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Setup error.");
        return 1;
    }
    rb.current_size = strlen(TEST_STRING);

    if(rb_advance(&rb,1) == false)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("rb_advance error.");
        return 1;
    }

    if(rb.start != 1)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("rb_advance error: rb.start(%ld) differs from expected(%d)",rb.start,1);
        return 1;
    }


    return 0;
}
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
        TRACE_ERROR_POSITION("Setup error.");
        return 1;
    }
    rb.current_size = strlen(TEST_STRING);

    uint8_t out[BUFFER_SIZE] = {0};
    if(rb_peek(&rb,out,test_size) == false)
    {
        TRACE_ERROR_POSITION("rb_peek error.");
        return 1;
    }

    if(memcmp(out,buffer,test_size) != 0)
    {
        TRACE_ERROR_POSITION("rb_peek error: memcmp failed");
        return 1;
    }

    return 0;
}
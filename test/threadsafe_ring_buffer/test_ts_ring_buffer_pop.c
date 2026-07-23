#include <internal/ts_ring_buffer.h>
#include <internal/ts_rb_message.h>
#include <internal/timestamp_gen.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "test_utils.h"

#define BUFFER_SIZE 100

int main()
{   
    const char *test = "Hello World";   
    uint8_t buffer[BUFFER_SIZE] = {0};
    uint16_t value = strlen(test);
    memcpy(buffer,&value,sizeof(value));
    memcpy(buffer+2,test,strlen(test));

    ts_ring_buffer_t tsrb = {0};
    ts_rb_setup(&tsrb,buffer,BUFFER_SIZE);
    tsrb.ring_buffer.current_size = strlen(test) + sizeof(uint16_t);
   
    size_t  popped_size = 0;
    uint8_t popped[BUFFER_SIZE] = {0};
    bool rc = ts_rb_pop(&tsrb,popped,BUFFER_SIZE,&popped_size);
    if(rc == false)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: ts_rb_pop failed!");
        return 1;
    }

    size_t expected_bytes = strlen(test);
    if(popped_size != expected_bytes)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: buffer size error!");
        TEST_ERROR("Found %ld instead of %ld",popped_size,expected_bytes);
        return 1;
    }

    if(strncmp(popped,test,strlen(test)) != 0)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: strncmp failed");
        return 1;
    }

    return 0;
}
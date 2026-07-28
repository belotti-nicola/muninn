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
    uint8_t  buffer[BUFFER_SIZE] = {0};
    uint16_t full_len = 4 + 2 + 11; // mask + test size + test
    uint16_t value = strlen(test);
    uint32_t mask = 16;
    memcpy(buffer,&full_len,sizeof(full_len));
    memcpy(buffer+2,&mask,sizeof(uint32_t));
    memcpy(buffer+6,&value,sizeof(uint16_t));
    memcpy(buffer+8,test,strlen(test));


    ts_ring_buffer_t tsrb = {0};
    ts_rb_setup(&tsrb,buffer,BUFFER_SIZE);
    tsrb.ring_buffer.current_size = strlen(test) + 8;
   
    size_t  popped_size = 0;
    uint8_t popped[BUFFER_SIZE] = {0};
    bool rc = ts_rb_pop(&tsrb,popped,BUFFER_SIZE,&popped_size);
    if(rc == false)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: ts_rb_pop failed!");
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
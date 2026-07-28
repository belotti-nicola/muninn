#include "internal/ts_ring_buffer.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "internal/timestamp_gen.h"

#include "test_utils.h"


#define BUFFER_SIZE 100

int main()
{   
    uint8_t rb_buffer[BUFFER_SIZE] = {0};
    ts_ring_buffer_t tsrb = {0};
    ts_rb_setup(&tsrb,rb_buffer,BUFFER_SIZE);

    char test[] = "test";

    bool push_rc = ts_rb_push(&tsrb,test,strlen(test));
    if(push_rc == false)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: ts_rb_push failed!");
        return 1;
    }

    size_t expected_bytes = strlen(test)+sizeof(uint16_t) + 4 + 2;
    if ( tsrb.ring_buffer.current_size != expected_bytes)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: rb size is %ld instead of the expected %ld!",tsrb.ring_buffer.current_size,expected_bytes);
        return 1;
    }
    
    if ( tsrb.ring_buffer.start != 0 )
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error start index error!");
        return 1;
    }
    
    size_t expected_end_index = expected_bytes;
    if ( tsrb.ring_buffer.end != expected_end_index )
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: ring buffer end index!");
        TEST_ERROR("Found %ld instead of %ld!",tsrb.ring_buffer.end,expected_end_index);
        return 1;
    }

    if( memcmp(tsrb.ring_buffer.data + 8,test,strlen(test)) != 0)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: strncmp fail!");
        return 1;
    }


    return 0;
}
#include <internal/protocols/muninn_messages/muninn_message_header.h>
#include <internal/protocols/muninn_messages/muninn_header_codec.h>

#define BUFFER_SIZE 1024

#include <stdint.h>
#include <stdbool.h>
#include "test_utils.h"


int main()
{
    uint8_t in[BUFFER_SIZE] = {0};
    in[0] = 215;
    in[1] = 7;
    size_t in_size = 2;

    muninn_header muninn_header;

    bool exit_code = muninn_header_decode(in,in_size,&muninn_header);
    if(exit_code == false)
    {
        TRACE_ERROR_POSITION();
        TEST_INFO("muninn_header_decode failed!");
        return 1;
    }

    if(muninn_header.payload_len != 2007)
    {
        TRACE_ERROR_POSITION();
        TEST_INFO("muninn_header.payload_len is %d instead of %d!",muninn_header.payload_len,2007);
        return 1;
    }
    
    return 0;
}
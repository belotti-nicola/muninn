#include <internal/protocols/muninn_messages/muninn_message_header.h>
#include <internal/protocols/muninn_messages/muninn_header_codec.h>

#define BUFFER_SIZE 1024

#include <stdint.h>
#include <stdbool.h>
#include "test_utils.h"


int main()
{
    uint8_t out[BUFFER_SIZE] = {0};
    size_t out_size = BUFFER_SIZE;

    muninn_header muninn_header;
    muninn_header.payload_len = 2007;

    bool exit_code = muninn_header_encode(&muninn_header,out,&out_size);
    if(exit_code == false)
    {
        TRACE_ERROR_POSITION();
        TEST_INFO("muninn_header_encode failed!");
        return 1;
    }

    if(out_size != sizeof(muninn_header))
    {
        TRACE_ERROR_POSITION();
        TEST_INFO("muninn_header_encode unexpected size(%ld)!",out_size);
        return 1;
    }

    if(out[0] != 215)
    {
        TRACE_ERROR_POSITION();
        TEST_INFO("muninn_header_encode byte unexpected value(%d instead of %d)!",215,(int)out[0]);
        return 1;
    }

    if(out[1] != 7)
    {
        TRACE_ERROR_POSITION();
        TEST_INFO("muninn_header_encode byte unexpected value(%d instead of %d)!",7,(int)out[0]);
        return 1;
    }

    
    return 0;
}
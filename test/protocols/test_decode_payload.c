#include <internal/protocols/muninn_messages/muninn_message_payload.h>
#include <internal/protocols/muninn_messages/muninn_payload_codec.h>
#include <internal/protocols/muninn_messages/muninn_message_mask.h>


#define BUFFER_SIZE 1024

#include <stdint.h>
#include <stdbool.h>
#include "test_utils.h"




int main()
{
    uint8_t in[BUFFER_SIZE] = {0};
    in[0] = 0;
    in[1] = 0;
    in[2] = 0;
    in[3] = 0;
    size_t in_size = 4;

    muninn_payload payload;
    bool exit_code = muninn_payload_decode(in,in_size,&payload);
    if(exit_code == false)
    {
        TRACE_ERROR_POSITION();
        TEST_INFO("muninn_payload_encode failed!");
        return 1;
    }

    
    return 0;
}

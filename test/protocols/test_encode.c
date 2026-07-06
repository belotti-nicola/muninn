#include <internal/protocols/messages/messages_codec.h>

#include "test_utils.h"

#define BUFFER_SIZE 256

int main()
{
    mnn_data_t mnn = 
    {       
        .line      = 2007,
        .timestamp = 1200,
        .thread_id = 12123,
        .severity  = 128,
        .pid       = 666,
     
        .file      = "file.c",
        .file_len  = sizeof("file.c") - 1,
        
        .func      = "function",
        .func_len  = sizeof("function") - 1,

        .msg       = "Hello World",
        .msg_len   = sizeof("Hello World") - 1
    };
    const ENCONDED_DATA_MASK mask = MEDM_ALL;

    uint8_t buffer[BUFFER_SIZE];
    size_t encoded_bytes;
    
    mm_encode(&mask,&mnn,buffer,BUFFER_SIZE,&encoded_bytes);

    if(encoded_bytes != 58)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Encoded bytes %ld differs from %d",encoded_bytes,58);
        return 1;
    }
   
}
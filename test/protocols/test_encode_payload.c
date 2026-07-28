#include <internal/protocols/muninn_messages/muninn_message_payload.h>
#include <internal/protocols/muninn_messages/muninn_payload_codec.h>
#include <internal/protocols/muninn_messages/muninn_message_mask.h>


#define BUFFER_SIZE 1024

#include <stdint.h>
#include <stdbool.h>
#include "test_utils.h"
#include <string.h>

#define PL_FILE     "file.c"
#define PL_FUNCTION "function"
#define PL_MESSAGE  "message"
#define PL_LINE     3077
#define PL_THREAD   122
#define PL_SEV      1
#define PL_PID      2
#define PL_TS       3



int main()
{
    uint8_t out[BUFFER_SIZE] = {0};
    size_t out_size = BUFFER_SIZE;

    muninn_payload payload;
    payload.file = PL_FILE;
    payload.file_len = strlen(PL_FILE);
    payload.func = PL_FUNCTION;
    payload.func_len = strlen(PL_FUNCTION);
    payload.line = PL_LINE;
    payload.msg = PL_MESSAGE;
    payload.msg_len  = strlen(PL_MESSAGE);
    payload.timestamp = PL_TS;
    payload.thread_id = PL_THREAD;
    payload.severity = PL_SEV;
    payload.pid = PL_PID;


    bool exit_code = muninn_payload_encode(&payload,MEDM_ALL,out,&out_size);
    if(exit_code == false)
    {
        TRACE_ERROR_POSITION();
        TEST_INFO("muninn_payload_encode failed!");
        return 1;
    }

    size_t expected_bytes = 4 // mask
        + sizeof(payload.timestamp)
        + sizeof(payload.thread_id)
        + sizeof(payload.line)
        + sizeof(payload.severity)
        + sizeof(payload.pid)
        + sizeof(payload.file_len)
        + sizeof(payload.func_len)
        + sizeof(payload.msg_len)
        + payload.file_len 
        + payload.func_len 
        + payload.msg_len;

    
    if(out_size != expected_bytes)
    {
        TRACE_ERROR_POSITION();
        TEST_INFO("muninn_payload_encode expected bytes number(%ld) differs from computed one (%ld!)",
            expected_bytes,
            out_size
        );
        return 1;
    }
    
    return 0;
}
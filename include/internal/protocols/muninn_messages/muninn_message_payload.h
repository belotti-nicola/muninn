#ifndef MUNINN_MESSAGE_PAYLOAD_H
#define MUNINN_MESSAGE_PAYLOAD_H

#include <stdint.h>

#include <internal/protocols/muninn_messages/muninn_message_mask.h>

typedef struct muninn_payload  
{
    muninn_message_mask mask;

    uint64_t timestamp;
    uint64_t thread_id;
    uint32_t line;
    uint8_t  severity;
    uint32_t pid;

    uint8_t  *file; 
    uint8_t   file_len;

    uint8_t  *func;
    uint8_t   func_len;

    uint8_t  *msg;
    uint16_t  msg_len;
    
} muninn_payload;

#endif
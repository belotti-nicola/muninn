#ifndef MUNINN_MESSAGE_H
#define MUNINN_MESSAGE_H

#include <stdint.h>

typedef struct muninn_message_t  
{
    uint64_t  timestamp;
    uint64_t  thread_id;
    uint32_t  line;
    uint8_t   severity;
    uint32_t  pid;

    const uint8_t  *file; 
    uint8_t         file_len;

    const uint8_t  *func;
    uint8_t         func_len;

    const uint8_t  *msg;
    uint16_t        msg_len;
    
} muninn_message_t;

#endif
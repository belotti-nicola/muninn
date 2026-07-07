#ifndef ENCODED_DATA_H
#define ENCODED_DATA_H

#include <stdint.h>
#include <stddef.h>

typedef struct mnn_data_t  
{
    //fixed lenght
    uint64_t timestamp;
    uint64_t thread_id;
    uint32_t line;
    uint8_t  severity;
    uint32_t pid;

    //variable
    uint8_t  *file; 
    uint8_t   file_len;

    uint8_t  *func;
    uint8_t   func_len;

    uint8_t  *msg;
    uint16_t  msg_len;
    
} mnn_data_t;

#endif
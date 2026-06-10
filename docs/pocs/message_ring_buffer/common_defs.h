#ifndef COMMON_DEFS_H
#define COMMON_DEFS_H

#include <stdint.h>

// BUFFER SIZE
#define RB_DATA_SIZE 1024


// so we can use sizeof 
typedef struct __attribute__((packed)) 
{
    uint32_t total_size;
    uint64_t timestamp;
    uint8_t  severity;

} message_header_t;


// COMPUTATION OF MAX SIZE
#define RB_MAX_PAYLOAD (RB_DATA_SIZE - sizeof(message_header_t))

//we put all together
typedef struct __attribute__((packed)) 
{
    message_header_t header;
    char             payload[RB_MAX_PAYLOAD];

} message_t;

#endif
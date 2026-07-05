#ifndef ENCODED_DATA_H
#define ENCODED_DATA_H

#include <stdint.h>
#include <stddef.h>

typedef struct mm_log_data_t 
{
    uint64_t timestamp;
    uint64_t thread_id;
    uint32_t line;
    uint8_t  severity;

    const char *file;
    const char *func;
    const char *msg;

} mm_log_data_t;

#endif
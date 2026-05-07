#ifndef LOG_QUEUE_H
#define LOG_QUEUE_H

#include <stdlib.h>

typedef struct log_message_t log_message_t;


typedef struct log_queue_t 
{
    log_message_t* head;
    log_message_t* tail;
    size_t size;

} log_queue_t;

#endif
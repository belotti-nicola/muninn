#include "internal/queue_message.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void setup_queue_message(queue_message_t *message,char *buffer, size_t max_size)
{
    message->size     = 0;
    message->max_size = max_size;
    message->data     = buffer;
    message->severity = LOG_DEBUG;

    memset(buffer,0,max_size);
}

void queue_message(queue_message_t *message,char *buffer, log_severity_t severity)
{
    int len            = strlen(buffer);
    message->size      = len;
    message->data      = buffer;
    message->data[len] = '\0';
    message->severity  = severity;
}



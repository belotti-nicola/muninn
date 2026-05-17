#ifndef QUEUE_MESSAGE_H
#define QUEUE_MESSAGE_H

#include <stddef.h>

typedef struct queue_message_t 
{
    char   *data;
    size_t  size;
    size_t  max_size;

} queue_message_t;

void setup_queue_message(queue_message_t *message,char *buffer, size_t max_size);
void       queue_message(queue_message_t *message,char *buffer);

#endif
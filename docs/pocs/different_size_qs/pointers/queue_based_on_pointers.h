#ifndef QUEUE_BASED_ON_POINTERS_H
#define QUEUE_BASED_ON_POINTERS_H

#include <stddef.h>

typedef struct message
{
    char   *buffer;
    size_t  buffer_dim;
    size_t  buffer_max_dim;

} message;

typedef struct queue
{
    message *messages;
    size_t   messages_dim;
    size_t   messages_max_dim;

} queue;

void setup_message(message *m,char *p, size_t max_dim);
void setup_queue(queue *q, message *m, size_t max_dim);


#endif
#include "queue_based_on_pointers.h"

#include <string.h>

void setup_message(message *m, char *buffer, size_t max_dim)
{
    m->buffer = buffer;
    m->buffer_dim = 0;
    m->buffer_max_dim = max_dim;

    memset(buffer,0,max_dim * sizeof(m));
}

void setup_queue(queue *q, message *m, size_t max_dim)
{
    q->messages = m;
    q->messages_dim = 0;
    q->messages_max_dim = max_dim;

    memset(q->messages,0,max_dim * sizeof(m));
}
#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdbool.h>
#include "queue_message.h"

#include <internal/munin_int.h>

typedef struct queue 
{
    size_t first_index;
    size_t last_index;
    size_t size;
    size_t max_size;

    queue_message_t *messages;

} queue_t;

void    queue_setup(queue_t *q,queue_message_t *buffer, size_t max_dim);
bool    queue_pop(queue_t* q,log_severity_t *severity, char *s);
bool    queue_push(queue_t* q,log_severity_t severity, const char *s);


#endif
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

    queue_message_t buffer[Q_SIZE];
    

} queue_t;

queue_t queue_setup();
bool    queue_pop(queue_t* q,queue_message_t *e);
bool    queue_push(queue_t* q,queue_message_t *e);


#endif
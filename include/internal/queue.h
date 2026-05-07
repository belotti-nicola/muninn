#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdbool.h>
#include "elem.h"


typedef struct queue 
{
    size_t           size;
    elem_t          *first;
    elem_t          *last;

} queue_t;

queue_t*               queue_create();
elem_t*                queue_pop(queue_t* q);
void                   queue_push(queue_t* q,elem_t *e);
void                   queue_destroy(queue_t *q);


#endif
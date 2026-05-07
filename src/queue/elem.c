#include <stdlib.h>
#include <stdio.h>

#include "internal/elem.h"

elem_t *elem_create(const char *msg, elem_t *next)
{
    elem_t *ptr = (elem_t *)malloc(sizeof(elem_t));
    if ( ptr == NULL )
    {
        printf("Error creating elem!");
        return NULL;
    }

    ptr->message = msg;
    ptr->next    = next;
    return ptr;
}
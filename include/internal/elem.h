#ifndef ELEM_H
#define ELEM_H

typedef struct elem 
{
    const char      *message;
    struct elem     *next;

} elem_t;

elem_t *elem_create(const char *msg,elem_t *next);

#endif
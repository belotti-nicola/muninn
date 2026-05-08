#include "internal/queue.h"
#include <string.h>
#include <stdlib.h>

int main()
{
    elem_t *tmp;
    queue_t *q;

    q = queue_create();
    if ( q == NULL )
    {
        printf("Error at line %d: queue is null!\n",__LINE__);
        return 1;
    }

    elem_t *el1 = elem_create("Hello",NULL);
    if ( el1 == NULL )
    {
        printf("Error at line %d: el1 is null!\n",__LINE__);
        return 1;
    }
    elem_t *el2 = elem_create("Hello",NULL);
    if ( el2 == NULL )
    {
        printf("Error at line %d: el2 is null!\n",__LINE__);
        return 1;
    }

    queue_push(q,el1);
    queue_push(q,el2);

    tmp = queue_pop(q);
    if ( el2 != q->first )
    {
        printf("Error at line %d: head is different\n",__LINE__);
    }
    if ( el2 != q->first )
    {
        printf("Error at line %d: head is different\n",__LINE__);
    }
    elem_destroy(tmp);

    tmp = queue_pop(q);
    elem_destroy(tmp);

    queue_destroy(q);
    return 0;
}
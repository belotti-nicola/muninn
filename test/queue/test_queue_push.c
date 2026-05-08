#include "internal/queue.h"
#include <string.h>
#include <stdlib.h>

bool check_q_push(queue_t *q,int value,int loc)
{
    int computed = q->size;
    if(q->size != value)
    {
        printf("Error at line %d: size is not %d(%d instead)!\n",loc,value,computed);
        return false;
    }

    return true;
}

int main()
{
    queue_t *q = queue_create();
    if ( q == NULL )
    {
        printf("Error at line %d: queue is null!\n",__LINE__);
        return 1;
    }

    if ( !check_q_push(q,0,__LINE__) )
    {
        return 1;
    }

    elem_t *tmp;
    tmp = elem_create("Hello",NULL);
    queue_push(q,tmp);
    if ( !check_q_push(q,1,__LINE__) )
    {
        return 1;
    }
    if ( tmp != q->first )
    {
        printf("Error at line %d: head is different\n",__LINE__);
    }
    if ( tmp != q->last )
    {
        printf("Error at line %d: last is different\n",__LINE__);
    }


    tmp = elem_create("World",NULL);
    queue_push(q,tmp);
    if ( !check_q_push(q,2,__LINE__) )
    {
        return 1;
    }
    if ( tmp != q->first )
    {
        printf("Error at line %d: head is different\n",__LINE__);
    }


    queue_destroy(q);
    return 0;
}
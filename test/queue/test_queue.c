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
    if ( !check_q_push(q,0,__LINE__) )
    {
        return 1;
    }

    elem_t *tmp = elem_create("Hello",NULL);

    tmp = elem_create("Hello",NULL);
    queue_push(q,tmp);
    if ( !check_q_push(q,1,__LINE__) )
    {
        return 1;
    }

    tmp = elem_create("World",NULL);
    queue_push(q,tmp);
    if ( !check_q_push(q,2,__LINE__) )
    {
        return 1;
    }


    tmp = queue_pop(q);
    if ( tmp == NULL )
    {
        printf("Error at line %d: found NULL elem!\n",__LINE__);
        return 1;
    }
    if ( tmp->message == NULL )
    {
        printf("Error at line %d: found NULL message!\n",__LINE__);
        return 1;
    }
    if ( strcmp(tmp->message,"Hello") == 1)
    {
        printf("Error at line %d: found %s instead of 'Hello'!\n",__LINE__,tmp->message);
        return 1;
    }


    queue_destroy(q);
    return 0;
}
#include "internal/ts_queue.h"
#include <string.h>
#include <stdlib.h>

int main()
{
    ts_queue_t q;
    ts_queue_setup(&q);
    queue_message_t qm = 
    {
        .message = "A"
    };
    bool cmd;
    
    cmd = ts_queue_push(&q,&qm);
    if(cmd == false)
    {
        printf("Error at line %d: queue failure!\n",__LINE__);
        return 1;
    }
    if(q.queue.size != 1)
    {
        printf("Error at line %d: size error(found %ld)!\n",__LINE__,q.queue.size);
        return 1;
    }

    if(q.queue.first_index != 0)
    {
        printf("Error at line %d: first index error!\n",__LINE__);
        return 1;
    }

    if(q.queue.last_index != 1)
    {
        printf("Error at line %d: last index error(found %ld)!\n",__LINE__,q.queue.last_index);
        return 1;
    }

    ts_queue_release(&q);
    return 0;
}
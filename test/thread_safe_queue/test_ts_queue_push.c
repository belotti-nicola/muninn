#include "internal/ts_queue.h"
#include <string.h>
#include <stdlib.h>

#define MESSAGE_SIZE 100
#define QUEUE_SIZE 5

int main()
{   
    int offset = 0;
    char buffer[MESSAGE_SIZE * QUEUE_SIZE]; 
    queue_message_t messages[QUEUE_SIZE];
    for(int i=0;i<QUEUE_SIZE;i++)
    {
        setup_queue_message(messages,buffer,MESSAGE_SIZE);
        offset += MESSAGE_SIZE;
    }
    queue_t q;
    queue_setup(&q,messages,QUEUE_SIZE);
    
    // ts_queue_t tsq;
    // ts_queue_setup(&tsq,)

    // queue_message_t qm = 
    // {
    //     .data = "A"
    // };
    // bool cmd;
    
    // cmd = ts_queue_push(&q,qm);
    // if(cmd == false)
    // {
    //     printf("Error at line %d: queue failure!\n",__LINE__);
    //     return 1;
    // }
    // if(q.size != 1)
    // {
    //     printf("Error at line %d: size error(found %ld)!\n",__LINE__,q.queue.size);
    //     return 1;
    // }

    // if(q.first_index != 0)
    // {
    //     printf("Error at line %d: first index error!\n",__LINE__);
    //     return 1;
    // }

    // if(q.queue.last_index != 1)
    // {
    //     printf("Error at line %d: last index error(found %ld)!\n",__LINE__,q.queue.last_index);
    //     return 1;
    // }

    //ts_queue_release(&q);
    return 0;
}
#include "internal/ts_queue.h"
#include <string.h>
#include <stdlib.h>

int main()
{
    ts_queue_t tsq = ts_queue_setup();
    queue_message_t qm = 
    {
        .message = "A"
    };
    bool cmd;
    
    ts_queue_push(&tsq,&qm);
    cmd = ts_queue_pop(&tsq,&qm);
    if(cmd == false)
    {
        printf("Error at line %d: queue failure!\n",__LINE__);
        return 1;
    }
    
    if(strcmp(qm.message,"A") == 1)
    {
        printf("Error at line %d: strcmp failed!\n",__LINE__);
        return 1;
    }


    ts_queue_release(tsq);
    return 0;
}
#include "internal/queue_rb.h"
#include <string.h>
#include <stdlib.h>

int main()
{
    queue_t q = queue_setup();
    queue_message_t qm = 
    {
        .message = "A"
    };
    bool cmd;
    
    cmd = queue_push(&q,&qm);
    if(cmd == false)
    {
        printf("Error at line %d: queue failure!\n",__LINE__);
        return 1;
    }

    if(q.first_index != 0)
    {
        printf("Error at line %d: first index error!\n",__LINE__);
        return 1;
    }

    if(q.last_index != 1)
    {
        printf("Error at line %d: last index error!\n",__LINE__);
        return 1;
    }

    if(q.size != 1)
    {
        printf("Error at line %d: last index error!\n",__LINE__);
        return 1;
    }

    return 0;
}
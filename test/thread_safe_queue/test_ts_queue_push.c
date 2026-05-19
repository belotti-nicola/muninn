#include "internal/ts_queue.h"
#include <string.h>
#include <stdlib.h>

#define MESSAGE_SIZE 100
#define QUEUE_SIZE 5

int main()
{   
    int offset = 0;
    char buffer[MESSAGE_SIZE * QUEUE_SIZE]; 
    queue_message_t messages[QUEUE_SIZE] = {0};
    for(int i=0;i<QUEUE_SIZE;i++)
    {
        setup_queue_message(messages+i,buffer+offset,MESSAGE_SIZE);
        offset += MESSAGE_SIZE;
    }

    ts_queue_t tsq;
    ts_queue_setup(&tsq,messages,QUEUE_SIZE);

    
    const char *test_case = "Hello World";
    bool push_rc = ts_queue_push(&tsq,test_case);
    if ( push_rc == false )
    {
        printf("Error at line %d: push returned false!\n",__LINE__);
        return 1;
    }
    if ( tsq.queue.size != 1)
    {
        printf("Error at line %d: q size is %ld instead of the expected 0!\n",__LINE__,tsq.queue.size);
        return 1;
    }
    if ( tsq.queue.first_index != 0 )
    {
        printf("Error at line %d: last index error!\n",__LINE__);
        return 1;
    }
    if ( tsq.queue.last_index != 1 )
    {
        printf("Error at line %d: last index error!\n",__LINE__);
        return 1;
    }

    queue_message_t *target = tsq.queue.messages + 0; //first
    if ( strcmp(target->data,test_case) != 0 )
    {
        printf("Error at line %d: strcmp failed: expected '%s', but found '%s' instead.\n",__LINE__,test_case,target->data);
        return 1;
    }

    ts_queue_release(&tsq);
    return 0;
}
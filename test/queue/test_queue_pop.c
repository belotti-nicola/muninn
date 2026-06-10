#include "internal/queue_rb.h"
#include <string.h>
#include <stdlib.h>

#define MESSAGE_SIZE LOG_MESSAGE_SIZE
#define QUEUE_SIZE LOG_QUEUE_SIZE

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
    queue_t q = {0};
    queue_setup(&q,messages,QUEUE_SIZE);
    if ( q.size != 0)
    {
        printf("Error at line %d: q size is %ld instead of the expected 0!\n",__LINE__,q.size);
        return 1;
    }
    
    char *test_case = "Hello World";
    bool rc = queue_push(&q,LOG_INFO,test_case);
    if ( q.size != 1)
    {
        printf("Error at line %d: q size is %ld instead of the expected 0!\n",__LINE__,q.size);
        return 1;
    }
    if ( rc == false )
    {
        printf("Error at line %d: push returned false!\n",__LINE__);
        return 1;
    }

    char popped_buffer[MESSAGE_SIZE] = {0};
    queue_message_t popped_message = {0};
    setup_queue_message(&popped_message,popped_buffer,MESSAGE_SIZE);
    rc = queue_pop(&q,&popped_message);
    if ( rc == false )
    {
        printf("Error at line %d: pop returned false!\n",__LINE__);
        return 1;
    }
    if(strlen(buffer) != popped_message.size )
    {
        printf("Error at line %d: strlen fail since '%ld'(expected) is different from '%ld'(computed value)!\n",__LINE__,strlen(buffer),popped_message.size);
        return 1;
    }

    if(strcmp(buffer,popped_buffer) != 0)
    {
        printf("Error at line %d: strcmp failed between '%s'(expected) and '%s'(computed value)!\n",__LINE__,buffer,popped_buffer);
        return 1;
    }

    return 0;
}
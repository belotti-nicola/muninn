#include "internal/queue_rb.h"
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
    int dim = strlen(test_case);
    char test_buffer[MESSAGE_SIZE];
    memcpy(test_buffer,test_case,dim);
    test_buffer[dim] = '\0';
    queue_message_t test_message = {0}; queue_message(&test_message,test_buffer);
    if( test_message.size != strlen(test_case) )
    {
        printf("Error at line %d: strlen is %ld and computed is'%ld'\n",__LINE__,strlen(test_case),test_message.size);
        return 1;
    }
    if ( strcmp(test_message.data,test_case) != 0 )
    {
        printf("Error at line %d: test message content is '%s' instead of the expected '%s'!\n",__LINE__,test_message.data,test_case);
        return 1;
    }

    bool push_rc = queue_push(&q,test_message);
    if ( push_rc == false )
    {
        printf("Error at line %d: push returned false!\n",__LINE__);
        return 1;
    }
    if ( q.size != 1)
    {
        printf("Error at line %d: q size is %ld instead of the expected 0!\n",__LINE__,q.size);
        return 1;
    }
    if (q.first_index != 0 )
    {
        printf("Error at line %d: last index error!\n",__LINE__);
        return 1;
    }
    if (q.last_index != 1 )
    {
        printf("Error at line %d: last index error!\n",__LINE__);
        return 1;
    }

    queue_message_t *pushed_message = q.messages;
    if( pushed_message->size != strlen(test_case) )
    {
        printf("Error at line %d: strlen is %ld and computed is'%ld'\n",__LINE__,strlen(test_case),pushed_message->size);
        return 1;
    }
    if ( strcmp(pushed_message->data,test_case) != 0 )
    {
        printf("Error at line %d: pushed message content is %s instead of the expected %s!\n",__LINE__,pushed_message->data,test_case);
        return 1;
    }


    return 0;
}
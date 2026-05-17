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
    queue_t q;
    queue_setup(&q,messages,QUEUE_SIZE);
    
    char *test_case = "Hello World";
    int dim = strlen(test_case);
    char test_buffer[MESSAGE_SIZE];
    memcpy(test_buffer,test_case,dim);
    test_buffer[dim] = '\0';
    queue_message_t test_message = {0}; queue_message(&test_message,test_buffer);
    bool rc = queue_push(&q,test_message);
    if ( rc == false )
    {
        printf("Error at line %d: push returned false!\n",__LINE__);
        return 1;
    }

    char popped_buffer[MESSAGE_SIZE] = {0};
    queue_message_t popped_message = {0};queue_message(&popped_message,popped_buffer);
    rc = queue_pop(&q,&popped_message);
    if ( rc == false )
    {
        printf("Error at line %d: pop returned false!\n",__LINE__);
        return 1;
    }
    if(popped_message.size != strlen(test_case))
    {
        printf("Error at line %d: strlen is %ld and computed is'%ld'\n",__LINE__,strlen(test_case),test_message.size);
        return 1;
    }
    if(strcmp(popped_message.data,test_case) != 0)
    {
        printf("Error at line %d: strcmp failed between '%s'(expected) and '%s'(concrete value)!\n",__LINE__,test_case,test_message.data);
        return 1;
    }

    return 0;
}
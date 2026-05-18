#include "internal/queue_rb.h"
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
    queue_t q = {0};
    queue_setup(&q,messages,QUEUE_SIZE);
    if ( q.size != 0)
    {
        printf("Error at line %d: q size is %ld instead of the expected 0!\n",__LINE__,q.size);
        return 1;
    }
    
    char *test_case = "Hello World";
    bool push_rc = queue_push(&q,test_case);
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

    return 0;
}
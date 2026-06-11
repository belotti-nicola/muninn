#include "internal/ts_ring_buffer.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 100

int main()
{   
    uint8_t buffer[BUFFER_SIZE] = {0};
    ts_ring_buffer_t tsrb = {0};

    ts_rb_setup(&tsrb,buffer,BUFFER_SIZE);

    char *test = "Hello World";
    bool push_rc = ts_rb_push(&tsrb,test,strlen(test));
    if(push_rc == false)
    {
        printf("Error at line %d: ts_rb_push failed!\n",__LINE__);
        return 1;
    }
    if ( tsrb.ring_buffer.current_size != strlen(test)+13)//TODO
    {
        printf("Error at line %d: rb size is %ld instead of the expected %ld!\n",__LINE__,tsrb.ring_buffer.current_size,strlen(test));
        return 1;
    }
    if ( tsrb.ring_buffer.start != 0 )
    {
        printf("Error at line %d: start index error!\n",__LINE__);
        return 1;
    }
    if ( tsrb.ring_buffer.end != strlen(test)+13 )//TODO
    {
        printf("Error at line %d: end index error!\n",__LINE__);
        return 1;
    }

    return 0;
}
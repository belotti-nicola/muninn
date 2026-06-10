#include <internal/ring_buffer.h>
#include <stdio.h>
#include <string.h>


#define BUFFER_SIZE 100

int  main()
{
    uint8_t buffer[BUFFER_SIZE] = {0};
    ring_buffer rb = {0};
    rb_setup(&rb,buffer,BUFFER_SIZE);

    char test1[80] = {0};
    char unused[80] = {0};

    rb_push(&rb,test1,80);
    rb_pop(&rb,unused,80);

    rb_push(&rb,test1,80);
    rb_pop(&rb,unused,80);

    if(rb.start != 61)
    {
        printf("Error at line %d: computed start(%ld) differs from expected one (%d)!\n",__LINE__,
            rb.start ,
            60);
        return 1;
    }

    return 0;
}
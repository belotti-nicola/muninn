#include <internal/ring_buffer.h>
#include <stdio.h>
#include <string.h>


#define BUFFER_SIZE 100

int  main()
{
    uint8_t buffer[BUFFER_SIZE] = {0};
    ring_buffer rb = {0};
    rb_setup(&rb,buffer,BUFFER_SIZE);

    char test1[55] = {0};
    char unused[55] = {0};

    rb_push(&rb,test1,55);
    rb_pop(&rb,unused,55);

    rb_push(&rb,test1,55);
    if(rb.end != 10)
    {
        printf("Error at line %d: computed end(%ld) differs from expected one (%d)!\n",__LINE__,
            rb.end ,
            10);
        return 1;
    }

    return 0;
}
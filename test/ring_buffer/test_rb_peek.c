#include <internal/ring_buffer.h>
#include <stdio.h>
#include <string.h>


#define BUFFER_SIZE 100

int  main()
{
    uint8_t buffer[BUFFER_SIZE] = {0};
    ring_buffer rb = {0};
    rb_setup(&rb,buffer,BUFFER_SIZE);

    char *test1 = "Hello";
    char *test2 = " ";
    char *test3 = "World";

    rb_push(&rb,test1,strlen(test1));
    if(rb_peek(&rb) != BUFFER_SIZE - strlen(test1))
    {
        printf("Error at line %d: computed available data(%ld) differs from expected one (%ld)!\n",__LINE__,
            rb_peek(&rb),
            BUFFER_SIZE - strlen(test1));
        return 1;
    }
    rb_push(&rb,test2,strlen(test2));
    if(rb_peek(&rb) != BUFFER_SIZE - strlen(test1) - strlen(test2))
    {
        printf("Error at line %d: computed available data(%ld) differs from expected one (%ld)!\n",__LINE__,
            rb_peek(&rb),
            BUFFER_SIZE - strlen(test1) - strlen(test2));
        return 1;
    }
    
    rb_push(&rb,test3, strlen(test3));
    if(rb_peek(&rb) != BUFFER_SIZE - strlen(test1) - strlen(test2) - strlen(test3))
    {
        printf("Error at line %d: computed available data(%ld) differs from expected one (%ld)!\n",__LINE__,
            rb_peek(&rb),
            BUFFER_SIZE - strlen(test1) - strlen(test2) - strlen(test3));
        return 1;
    }

    char unused[BUFFER_SIZE] = {0};
    rb_pop(&rb,unused,10);
    if(rb_peek(&rb) != 99)
    {
        printf("Error at line %d: computed available data(%ld) differs from expected one (%d)!\n",__LINE__,
            rb_peek(&rb),
            99);
        return 1;
    }

    rb_pop(&rb,unused,1);
    if(rb_peek(&rb) != 100)
    {
        printf("Error at line %d: computed available data(%ld) differs from expected one (%d)!\n",__LINE__,
            rb_peek(&rb),
            100);
        return 1;
    }

    return 0;
}
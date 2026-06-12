#include <internal/ts_ring_buffer.h>
#include <internal/ts_rb_message.h>
#include <internal/timestamp_gen.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 100

int main()
{   
    uint32_t len = 24;
    uint64_t ts  = timestamp_u64();
    uint8_t  sev = 127;
    const char *test = "Hello World";   
    uint8_t buffer[BUFFER_SIZE] = {0};
    memcpy(buffer,&len,sizeof(uint32_t));
    memcpy(buffer+4,&ts ,sizeof(uint64_t));
    memcpy(buffer+12,&sev,sizeof(uint8_t));
    memcpy(buffer+13,test,strlen(test));


    ts_ring_buffer_t tsrb = {0};
    ts_rb_setup(&tsrb,buffer,BUFFER_SIZE);
    tsrb.ring_buffer.current_size = strlen(test) + sizeof(uint32_t) + sizeof(uint64_t) + sizeof(uint8_t);
   
    uint8_t popped[BUFFER_SIZE] = {0};
    ts_rb_message_t message;ts_rb_message_setup(&message,popped,BUFFER_SIZE);
    bool rc = ts_rb_pop(&tsrb,&message);
    if(rc == false)
    {
        printf("Error at line %d: ts_rb_pop failed!\n",__LINE__);
        return 1;
    }

    if(message.header.msg_len != 24)
    {
        printf("Error at line %d: msg_len(%d) differs from expected(%d)!\n",__LINE__,message.header.msg_len,24);
        return 1;
    }
    if(message.header.severity != 127)
    {
        printf("Error at line %d: severity(%d) differs from expected(%d)!\n",__LINE__,message.header.msg_len,24);
        return 1;
    }
    if(message.header.ts != ts)
    {
        printf("Error at line %d: timestamp(%d) differs from expected(%d)!\n",__LINE__,message.header.msg_len,24);
        return 1;
    }
    if(strncmp(message.payload.payload_bytes,test,strlen(test)) != 0)
    {
        printf("Error at line %d: strncmp failed\n",__LINE__);
        return 1;
    }

    return 0;
}
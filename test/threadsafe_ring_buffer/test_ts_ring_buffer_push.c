#include "internal/ts_ring_buffer.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "internal/timestamp_gen.h"


#define BUFFER_SIZE 100

int main()
{   
    uint8_t rb_buffer[BUFFER_SIZE] = {0};
    ts_ring_buffer_t tsrb = {0};
    ts_rb_setup(&tsrb,rb_buffer,BUFFER_SIZE);

    char *test = "Hello World";
    ts_rb_message_t message;
    uint64_t ts = timestamp_u64();
    ts_rb_message_set(&message,strlen(test)+sizeof(ts_rb_header_t),ts,127,test);

    bool push_rc = ts_rb_push(&tsrb,&message);
    if(push_rc == false)
    {
        printf("Error at line %d: ts_rb_push failed!\n",__LINE__);
        return 1;
    }
    if ( tsrb.ring_buffer.current_size != strlen(test)+13)
    {
        printf("Error at line %d: rb size is %ld instead of the expected %ld!\n",__LINE__,tsrb.ring_buffer.current_size,strlen(test)+13);
        return 1;
    }
    if ( tsrb.ring_buffer.start != 0 )
    {
        printf("Error at line %d: start index error!\n",__LINE__);
        return 1;
    }
    if ( tsrb.ring_buffer.end != strlen(test)+13 )
    {
        printf("Error at line %d: end index error!\n",__LINE__);
        return 1;
    }
  

    uint32_t tmp_len;
    memcpy(&tmp_len,rb_buffer,4);
    if(memcmp(rb_buffer,&tmp_len,4) != 0)
    {
        printf("Error at line %d: len error!\n",__LINE__);
        return 1;
    }

    uint64_t tmp_ts;
    memcpy(&tmp_ts,rb_buffer+4,sizeof(uint64_t));
    if(memcmp(rb_buffer+4,&tmp_ts,sizeof(uint64_t)) != 0)
    {
        printf("Error at line %d: ts error!\n",__LINE__);
        return 1;
    }

    uint8_t tmp_sev;
    memcpy(&tmp_sev,rb_buffer+12,sizeof(uint8_t));
    if(memcmp(rb_buffer+12,&tmp_sev,sizeof(uint8_t)) != 0)
    {
        printf("Error at line %d: found severity %d instead of expected %d!\n",__LINE__,tmp_sev,127);
        return 1;
    }

    if( memcmp(tsrb.ring_buffer.data + 13,test,strlen(test)) != 0)
    {
        printf("Error at %s:%d: strncmp fail!\n",__FILE__,__LINE__);
        return 1;
    }


    return 0;
}
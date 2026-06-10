#include "ring_buffer.h"
#include "common_defs.h"
#include "timestamp.h"

#include <string.h>

void rb_setup(ring_buffer *rb)
{
    rb->start = 0;
    rb->end = 0;
    rb->cur_size = 0;
    memset(rb->data,0,RB_DATA_SIZE);
}


void rb_push(ring_buffer *rb,uint8_t severity,const char *msg)
{
    size_t available_bytes = RB_DATA_SIZE - rb->cur_size;
    if( available_bytes   <= sizeof(message_header_t) )
    {
        //drop it: no payload bytes can be stored
        return;
    }

    uint32_t msg_len = (uint32_t)strlen(msg) + sizeof(message_header_t);
    uint64_t ts      = timestamp_generation();

    size_t bytes_to_copy = 0;
    size_t bytes_copied = 0;
    size_t offset = 0;
    char *target;

    bytes_to_copy = sizeof(uint32_t);
    uint32_t *msg_len_ptr = &msg_len;
    offset = rb->end;
    if(offset == RB_DATA_SIZE - 1) { offset = 0;}
    target = rb->data + offset;
    if(offset + bytes_to_copy >= RB_DATA_SIZE) 
    {
        size_t available_bytes = RB_DATA_SIZE - offset;
        memcpy(target,msg_len_ptr,available_bytes);
        msg_len_ptr   += available_bytes;
        bytes_copied  += available_bytes;
        target         = rb->data;
    }
    memcpy(target,msg_len_ptr,bytes_to_copy - bytes_copied);
    rb->end      += bytes_to_copy;
    rb->cur_size += bytes_to_copy;

    bytes_to_copy = sizeof(uint64_t);
    bytes_copied  = 0;
    uint64_t *ts_ptr = &ts;
    offset = rb->end;
    if(offset == RB_DATA_SIZE - 1) { offset = 0;}
    target = rb->data + offset;
    if(offset + bytes_to_copy >= RB_DATA_SIZE) 
    {
        size_t available_bytes = RB_DATA_SIZE - offset;
        memcpy(target,ts_ptr,available_bytes);
        msg_len_ptr   += available_bytes;
        bytes_copied  += available_bytes;
        target         = rb->data;
    }
    memcpy(target,ts_ptr,bytes_to_copy - bytes_copied);
    rb->end      += bytes_to_copy;
    rb->cur_size += bytes_to_copy;


    bytes_to_copy = sizeof(uint8_t);
    uint8_t *sev_ptr = &severity;
    offset = rb->end;
    if(offset == RB_DATA_SIZE - 1) { offset = 0;}
    target = rb->data + offset;
    if(offset + bytes_to_copy >= RB_DATA_SIZE) 
    {
        target = rb->data;
    }
    memcpy(target,sev_ptr,bytes_to_copy);
    rb->end      += bytes_to_copy;
    rb->cur_size += bytes_to_copy;


    bytes_to_copy = strlen(msg);
    bytes_copied  = 0;
    offset = rb->end;
    target = rb->data + offset;
    if(offset + bytes_to_copy >= RB_DATA_SIZE) 
    {
        size_t available_bytes = RB_DATA_SIZE - offset;
        memcpy(target,msg,available_bytes);
        msg           += available_bytes;
        bytes_copied  += available_bytes;
        target         = rb->data;
    }
    memcpy(target,msg,bytes_to_copy - bytes_copied);
    rb->end      += bytes_to_copy;
    rb->cur_size += bytes_to_copy;
}

void rb_pop(ring_buffer *rb, message_t *out)
{
    size_t start = rb->start;
    size_t bytes_to_read = 0;
    size_t read_bytes = 0;
    
    char     *tmp = &rb->data[start];

    uint32_t  message_len; 
    uint32_t *message_len_ptr = &message_len;
    bytes_to_read = sizeof(uint32_t);
    if ( start + bytes_to_read > RB_DATA_SIZE - 1 )
    {
        size_t available_bytes = RB_DATA_SIZE - start;
        memcpy(message_len_ptr,tmp,available_bytes);
        message_len_ptr    += available_bytes;
        start               = 0;
        read_bytes         += available_bytes;
    }
    memcpy(message_len_ptr,tmp, 4);
    out->header.total_size = message_len;
    tmp += sizeof(uint32_t);
    start += sizeof(uint32_t);

    uint64_t  ts; 
    uint64_t *ts_ptr = &ts;
    bytes_to_read = sizeof(uint64_t);
    if ( start + bytes_to_read > RB_DATA_SIZE - 1 )
    {
        size_t available_bytes = RB_DATA_SIZE - start;
        memcpy(ts_ptr,tmp,available_bytes);
        message_len_ptr    += available_bytes;
        start               = 0;
        read_bytes         += available_bytes;
    }
    memcpy(ts_ptr,tmp,bytes_to_read);
    out->header.timestamp = ts;
    tmp += sizeof(uint64_t);
    start += sizeof(uint64_t);

    uint8_t  sev; 
    uint8_t *sev_ptr = &sev;
    bytes_to_read = sizeof(uint8_t);
    if ( start + bytes_to_read > RB_DATA_SIZE - 1 )
    {
        start               = 0;
    }
    memcpy(sev_ptr,tmp,bytes_to_read);
    out->header.severity = sev;
    tmp += sizeof(uint8_t);
    start += sizeof(uint8_t);


    bytes_to_read = message_len - sizeof(message_header_t);
    if ( start + bytes_to_read > RB_DATA_SIZE - 1 )
    {
        size_t available_bytes = RB_DATA_SIZE - start;
        memcpy(&out->payload,tmp,available_bytes);
        bytes_to_read -= available_bytes;
    }
    memcpy(&out->payload,tmp,bytes_to_read);
    
    rb->start    += message_len;
    rb->cur_size -= message_len;
    
}
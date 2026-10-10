#include <internal/gateway_th.h>
#include <internal/flogger_th.h>
#include <muninn.h>
#include <internal/timestamp_gen.h>
#include <string.h>
#include <stdio.h>

#include <internal/protocols/muninn_messages/muninn_codec.h>
#include <internal/protocols/muninn_messages/muninn_header_codec.h>
#include <internal/protocols/muninn_messages/muninn_payload_codec.h>
#include <internal/protocols/muninn_messages/muninn_message.h>

#include <internal/timestamp_gen.h>

#include <internal/muninn_message.h>


#define MESSAGE_LEN 2048

void *gateway_loop_fn(void *arg)
{
    if (arg == NULL) return NULL;
    gateway_th_data *gw_data = (gateway_th_data *)arg;
    
    ts_queue_t *q1 = gw_data->q1; 
    ts_queue_t *q2 = gw_data->q2; 
    ts_ring_buffer_t *rb = gw_data->rb;
    
    uint8_t read_buffer[LOG_MESSAGE_SIZE] = {0};

    muninn_message message = {0};
    muninn_header header   = {0};
    muninn_payload payload = {0};

    uint8_t payload_msg[MESSAGE_LEN] = {0}; 

    payload.msg = payload_msg;
    
    message.header  = &header;
    message.payload = &payload;
    

    size_t header_size     = sizeof(muninn_header);

    while ( true )
    {
        if ( ts_rb_peek( rb, read_buffer, header_size) == false )
        {
            break;
        }

        if ( muninn_header_decode(read_buffer,header_size,&header) == false )
        {
            break;
        }

        size_t message_size = (size_t)header.payload_len + header_size ;

        if ( ts_rb_peek( rb, read_buffer, message_size) == false )
        {
            break;
        }

        if ( muninn_messages_decode(read_buffer,message_size,&message) == false )
        {
            break;
        }

        if ( ts_rb_advance( rb, message_size ) == false )
        {
            break;
        }

        ts_queue_n_push(q1, 1, message.payload->msg, message.payload->msg_len);
        ts_queue_n_push(q2, 1, message.payload->msg, message.payload->msg_len);
    }

    printf("Gateway end\n");
    return NULL;
}
void *gateway_stop_fn(void *arg)
{
    if(arg == NULL) return NULL;
    gateway_th_data *gw_data = (gateway_th_data *)arg;
    
    if(gw_data->rb == NULL) return NULL;

    ts_rb_stop(gw_data->rb);
    return NULL;
}

void *gateway_post_fn(void *context, void *data, size_t data_size)
{
    if(context == NULL || data == NULL) return NULL;
    
    gateway_th_data *gw_data = (gateway_th_data *)context;
    muninn_message_t *msg    = (muninn_message_t *)data;

    if(gw_data->rb == NULL) return NULL;
    ts_ring_buffer_t *tsrb = gw_data->rb;

    muninn_encoding_mask_t *gw_mask = gw_data->mask;
    muninn_message_mask mask        = muninn_encoding_mask_get(gw_mask);


    muninn_header header = 
    {
        .payload_len = 2 + data_size
    };

    muninn_payload payload = 
    {
        .msg_len = msg->msg_len,
        .msg = msg->msg,
        .timestamp = msg->timestamp,
        .thread_id = msg->thread_id,
        .line = msg->line,
        .severity = msg->severity,
        .pid = msg->pid,
        .file = msg->file,
        .file_len = msg->file_len,

        .mask = mask
    };
    muninn_message mm = 
    {
        .header  = &header,
        .payload = &payload
    };

    uint8_t buffer[2048] = {0};
    size_t buffer_size   = 2048;
    if(muninn_messages_encode(&mm,buffer,&buffer_size) == false)
    {
        return NULL;
    }

    ts_rb_push(tsrb,buffer,buffer_size);

    return NULL;
}
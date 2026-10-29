#include <internal/gateway_th.h>
#include <internal/flogger_th.h>
#include <muninn.h>
#include <internal/timestamp_gen.h>
#include <string.h>
#include <stdio.h>

#include <internal/protocols/muninn_encoded_messages/muninn_encoded_message_codec.h>
#include <internal/protocols/muninn_encoded_messages/muninn_encoded_message_header_codec.h>
#include <internal/protocols/muninn_encoded_messages/muninn_encoded_message_payload_codec.h>
#include <internal/protocols/muninn_encoded_messages/muninn_encoded_message.h>

#include <internal/protocols/gateway_message_codec/gateway_message_codec.h>

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

    uint8_t func_buffer[256] = {0};
    uint8_t file_buffer[256] = {0};
    uint8_t  msg_buffer[256] = {0};


    muninn_message message = {0};

    muninn_header header   = {0};
    muninn_payload payload = {0};

    payload.func = func_buffer;
    payload.file = file_buffer;
    payload.msg  =  msg_buffer;


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

        if ( ts_rb_advance( rb, message_size ) == false )
        {
            break;
        }

        ts_queue_n_push(q1, read_buffer, message_size );
        ts_queue_n_push(q2, read_buffer, message_size );
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
    uint8_t *message         = (uint8_t *)data;

    if(gw_data->rb == NULL) return NULL;
    ts_ring_buffer_t *tsrb = gw_data->rb;

    if ( gateway_message_codec_prependable_size(data_size,message,2048,NULL) == false )
    {
        return NULL;
    }

    ts_rb_push(tsrb,message,data_size+2);

    return NULL;
}
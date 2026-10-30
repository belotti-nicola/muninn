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
    
    uint8_t read_buffer[LOG_MESSAGE_SIZE] = {0}; // todo

    size_t header_size      = sizeof(uint16_t); // todo
    size_t read_buffer_size = 0; 

    while ( true )
    {
        if ( ts_rb_peek( rb, read_buffer, header_size) == false )
        {
            break;
        }

        if ( gateway_message_codec_decode_message_size(read_buffer,&read_buffer_size) == false )
        {
            break;
        }

        if ( ts_rb_advance( rb,read_buffer_size ) == false )
        {
            break;
        }

        ts_queue_n_push(q1, read_buffer + header_size , read_buffer_size - header_size);
        ts_queue_n_push(q2, read_buffer + header_size , read_buffer_size - header_size);


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

    ts_rb_push(tsrb,message,data_size);

    return NULL;
}
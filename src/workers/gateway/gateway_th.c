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

void *gateway_loop_fn(void *arg)
{
    if (arg == NULL) return NULL;
    gateway_th_data *gw_data = (gateway_th_data *)arg;
    
    ts_queue_t *q1 = gw_data->q1; 
    ts_queue_t *q2 = gw_data->q2; 
    ts_ring_buffer_t *rb = gw_data->rb;
    
    size_t  read_buffer_bytes = 0;
    uint8_t read_buffer[LOG_MESSAGE_SIZE] = {0};

    while (ts_rb_pop(rb, read_buffer, LOG_MESSAGE_SIZE, &read_buffer_bytes))
    {
        ts_queue_n_push(q1, 1, read_buffer, read_buffer_bytes);
        ts_queue_n_push(q2, 1, read_buffer, read_buffer_bytes);   
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
    const char *message_data = (const char *)data;

    if(gw_data->rb == NULL) return NULL;
    ts_ring_buffer_t *tsrb = gw_data->rb;


    muninn_header header = 
    {
        .payload_len = 2 + data_size
    };

    muninn_payload payload = 
    {
        .msg = (uint8_t *)message_data,
        .msg_len = data_size,
        .mask = MEDM_MESSAGE
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
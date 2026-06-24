#include <internal/gateway_th.h>
#include <internal/flogger_th.h>

#include <muninn.h>
#include <internal/timestamp_gen.h>

#include <string.h>

#define BUFFSIZE 4096

void *gateway_loop_fn(void *arg)
{
    if(arg == NULL) return NULL;
    gateway_th_data *gw_data = (gateway_th_data *)arg;
    ts_queue_t *q1 = gw_data->q1;
    ts_queue_t *q2 = gw_data->q2;

    
    uint8_t read_buffer[LOG_MESSAGE_SIZE];
    ts_rb_message_t message;
    ts_rb_message_setup(
        &message,
        read_buffer,
        LOG_MESSAGE_SIZE
    );

    ts_ring_buffer_t *rb = gw_data->rb;
    if(rb == NULL) {return NULL;}


    char buffer[BUFFSIZE] = {0};
    size_t buffer_size = 0;
    while(ts_rb_pop(rb,&message))
    {
        if(13 + buffer_size + message.header.msg_len + 1 > BUFFSIZE) continue;

        memcpy(buffer + buffer_size, message.payload.payload_bytes, message.header.msg_len);
        buffer[message.header.msg_len-13 + buffer_size] = '\n';
        buffer_size += message.header.msg_len + 1 -13;

        if(buffer_size < BUFFSIZE * 0.95 ) continue;

        ts_queue_push(q1,1,buffer);
        ts_queue_push(q2,1,buffer);
        
        buffer[0] = '\0';
        buffer_size = 0;
    }

    if(buffer_size > 0 )
    {
        ts_queue_push(q1,1,buffer);
        ts_queue_push(q2,1,buffer);
    }

    printf("Gateway end\n");
    return NULL;
}

void *gateway_stop_fn(void *arg)
{
    if(arg == NULL) return NULL;

    gateway_th_data *gw_data = (gateway_th_data *)arg;
    if(&gw_data->rb == NULL) return NULL;

    ts_rb_stop(gw_data->rb);

    return NULL;
}

void *gateway_post_fn(void *context, void *data)
{
    if(context == NULL || data == NULL) return NULL;
    
    gateway_th_data *gw_data = (gateway_th_data *)context;
    const char *message_data = (const char *)data;

    if(gw_data->rb == NULL) return NULL;
    ts_ring_buffer_t *tsrb = gw_data->rb;

    ts_rb_message_t message = {0};
    size_t full_len = strlen(message_data) + sizeof(ts_rb_header_t);
    uint64_t ts = timestamp_u64();
    uint8_t sev = (uint8_t)127;//TODO
    ts_rb_message_set(&message,full_len,ts,sev,(uint8_t *)message_data);

    ts_rb_push(tsrb,&message);

    return NULL;
}
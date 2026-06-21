#include <internal/gateway_th.h>
#include <internal/flogger_th.h>

#include <muninn.h>
#include <internal/timestamp_gen.h>

#include <string.h>

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
    char with_terminator[4096];//todo
    while(ts_rb_pop(rb,&message))
    {
        //TODO
        strncat(with_terminator,message.payload.payload_bytes,message.header.msg_len);
        with_terminator[message.header.msg_len] = '\0';

        ts_queue_push(q1,1,with_terminator);
        ts_queue_push(q2,1,with_terminator);
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
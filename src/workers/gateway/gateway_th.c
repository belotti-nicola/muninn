#include <internal/gateway_th.h>
#include <internal/flogger_th.h>

#include <muninn.h>
#include <internal/timestamp_gen.h>

#include <string.h>

void *gateway_loop_fn(void *arg)
{
    if(arg == NULL) return NULL;
    gateway_th_data *gw_data = (gateway_th_data *)arg;

    muninn_t *m = gw_data->muninn;
    if(m == NULL) return NULL;
    
    ts_ring_buffer_t *rb     = &m->gateway_rb;
    ts_rb_message_t *message = &m->gateway_message;
    ts_rb_message_setup(
        message,
        (uint8_t *)&m->gateway_message,
        LOG_MESSAGE_SIZE
    );


    char with_terminator[4096];
    while(ts_rb_pop(rb,message))
    {
        //TODO
        strncat(with_terminator,message->payload.payload_bytes,message->header.msg_len);
        with_terminator[message->header.msg_len] = '\0';

        flogger_post_fn(&m->flogger_th,with_terminator);//TODO
        console_th_perform(&m->console_th,1,with_terminator);//TODO
    }
    return NULL;
}

void *gateway_stop_fn(void *arg)
{
    if(arg == NULL) return NULL;

    gateway_th_data *gw_data = (gateway_th_data *)arg;

    return NULL;
}

//TODO
void *gateway_join_fn(void *arg)
{
    if(arg == NULL) return NULL;

    gateway_th_data *gw_data = (gateway_th_data *)arg;

    return NULL;
}

void *gateway_th_perform(void *context, void *data)
{
    if(context == NULL || data == NULL) return NULL;
    
    gateway_th_data *gw_data = (gateway_th_data *)context;
    const char *message_data = (const char *)data;

    if(gw_data->muninn == NULL) return NULL;

    ts_ring_buffer_t *tsrb = &gw_data->muninn->gateway_rb;
    if(tsrb == NULL) return NULL;

    ts_rb_message_t message = {0};
    size_t full_len = strlen(message_data) + sizeof(ts_rb_header_t);
    uint64_t ts = timestamp_u64();
    uint8_t sev = (uint8_t)127;//TODO
    ts_rb_message_set(&message,full_len,ts,sev,(uint8_t *)message_data);

    ts_rb_push(tsrb,&message);

    return NULL;
}
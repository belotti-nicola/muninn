#include <internal/gateway_th.h>
#include <muninn.h>

void *gateway_loop_fn(void *arg)
{
    if(arg == NULL) return NULL;
    gateway_th_data *gw_data = (gateway_th_data *)arg;

    muninn_t *m = (muninn_t *)gw_data->context;
    if(m == NULL) return NULL;
    
    ts_ring_buffer_t *rb     = &m->gateway_rb;
    ts_rb_message_t *message = &m->gateway_message;
    ts_rb_message_setup(
        message,
        (uint8_t *)m->gateway_buff,
        LOG_RB_SIZE
    );



    while(ts_rb_pop(rb,message))
    {
        //logger_th_perform(m->logger_th,message->header.severity,message->payload);

    }
    return NULL;
}

void *gateway_stop_fn(void *arg)
{
    if(arg == NULL) return NULL;

    gateway_th_data *gw_data = (gateway_th_data *)arg;
}

void *gateway_join_fn(void *arg)
{
    if(arg == NULL) return NULL;

    gateway_th_data *gw_data = (gateway_th_data *)arg;
}

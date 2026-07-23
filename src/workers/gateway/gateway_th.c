#include <internal/gateway_th.h>
#include <internal/flogger_th.h>
#include <muninn.h>
#include <internal/timestamp_gen.h>
#include <string.h>
#include <stdio.h>

void *gateway_loop_fn(void *arg)
{
    if(arg == NULL) return NULL;
    gateway_th_data *gw_data = (gateway_th_data *)arg;
    
    ts_queue_t *q1 = gw_data->q1; // Coda File
    ts_queue_t *q2 = gw_data->q2; // Coda Console
    ts_ring_buffer_t *rb = gw_data->rb;
    

    size_t  read_buffer_bytes = 0;
    uint8_t read_buffer[LOG_MESSAGE_SIZE] = {0};
    while(ts_rb_pop(rb,read_buffer,LOG_MESSAGE_SIZE,&read_buffer_bytes))
    {
        ts_queue_n_push(q1, 1, read_buffer,read_buffer_bytes);
        ts_queue_n_push(q2, 1, read_buffer,read_buffer_bytes);
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

    ts_rb_push(tsrb,message_data,data_size);

    return NULL;
}
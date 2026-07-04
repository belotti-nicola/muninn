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
    
    //if(rb == NULL || q1 == NULL || q2 == NULL) return NULL;

    // Allocazione del buffer singolo per estrarre un messaggio alla volta
    uint8_t read_buffer[LOG_MESSAGE_SIZE];
    ts_rb_message_t message;
    ts_rb_message_setup(&message, read_buffer, LOG_MESSAGE_SIZE);

    // Il loop ora fa solo da passacarte: POP -> PUSH. Zero memcpy intermedie.
    while(ts_rb_pop(rb, &message))
    {
        // Protezione contro l'underflow dei 13 byte
        if (message.header.msg_len >= 13) 
        {
            message.payload.payload_bytes[message.header.msg_len - 13] = '\0';
        }
        else 
        {
            // Se il messaggio è anomalo e corto, ci assicuriamo che sia comunque chiuso
            message.payload.payload_bytes[message.header.msg_len] = '\0';
        }

        // 1. Spingiamo il singolo messaggio sulla coda del File Logger (Bloccante e sicuro)
        ts_queue_push(q1, 1, (char *)message.payload.payload_bytes);
        ts_queue_push(q2, 1, (char *)message.payload.payload_bytes);

        // 2. OTTIMIZZAZIONE TURBO: Per la console (q2), usiamo una try_push non bloccante.
        // Se la console è lenta o intasata, il Gateway non si ferma a dormire, 
        // scarta il log a schermo e continua a servire il File Logger a milioni di log al secondo.
        // if (!ts_queue_try_push(q2, 1, (char *)message.payload.payload_bytes))
        // {
        //     // Opzionale: qui puoi incrementare un contatore di log saltati in console
        // }
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
    uint8_t sev = (uint8_t)127; 
    
    ts_rb_message_set(&message, full_len, ts, sev, (uint8_t *)message_data);
    ts_rb_push(tsrb, &message);

    return NULL;
}
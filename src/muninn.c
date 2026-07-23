#include <muninn.h>

#include "internal/muninn_int.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <internal/timestamp_gen.h>
#include <internal/compressor_th.h>
#include <internal/flogger_th.h>
#include <internal/panic_flusher.h>

#include <internal/gateway_th.h>
#include <internal/flogger_th.h>
#include <internal/clogger_th.h>
#include <internal/compressor_th.h>

#include <internal/protocols/messages/messages_codec.h>



bool muninn_init(muninn_t *muninn, CONFIG config)
{
    if(muninn == NULL) return false;
    memset(muninn, 0, sizeof(muninn_t));

    muninn->start_ts = timestamp_u64();
    size_t offset;

    // ==========================================
    // FLOGGER (FILE)
    // ==========================================
    if (config.file.enabled) 
    {
        // Copia sicura del path del file
        strncpy(muninn->flogger_th.path, config.file.path, P_SIZE - 1);
        muninn->flogger_th.path[P_SIZE - 1] = '\0';

        offset = 0;
        for(int i=0; i<LOG_QUEUE_SIZE; i++) {
            char *tmp = muninn->flogger_buff + offset;
            setup_queue_message(muninn->flogger_m+i, tmp, LOG_MESSAGE_SIZE);
            offset += LOG_MESSAGE_SIZE;
        }
        ts_queue_setup(&muninn->flogger_q, muninn->flogger_m, LOG_QUEUE_SIZE);
    }

    // ==========================================
    // COMPRESSOR
    // ==========================================
    if (config.compressor.enabled) 
    {
        offset = 0;
        for(int i=0; i<COMP_QUEUE_SIZE; i++) {
            char *tmp = muninn->fcompressor_buff + offset;
            setup_queue_message(muninn->fcompressor_m+i, tmp, COMP_MESSAGE_SIZE);
            offset += COMP_MESSAGE_SIZE;
        }
        ts_queue_setup(&muninn->fcompressor_q, muninn->fcompressor_m, COMP_QUEUE_SIZE);
    }

    // ==========================================
    // CLOGGER (CONSOLE)
    // ==========================================
    if (config.console.enabled) 
    {
        offset = 0;
        for(int i=0; i<CONS_QUEUE_SIZE; i++) {
            char *tmp = muninn->clogger_buff + offset;
            setup_queue_message(muninn->clogger_m+i, tmp, CONS_MESSAGE_SIZE);
            offset += CONS_MESSAGE_SIZE;
        }
        ts_queue_setup(&muninn->clogger_q, muninn->clogger_m, CONS_QUEUE_SIZE);
    }

    // ==========================================
    // GATEWAY 
    // ==========================================
    ts_rb_setup(&muninn->gateway_rb, (uint8_t *)muninn->gateway_buff, LOG_RB_SIZE);
    muninn->gateway_th.rb = &muninn->gateway_rb;
    
    muninn->gateway_th.q1 = config.file.enabled    ? &muninn->flogger_q : NULL;
    muninn->gateway_th.q2 = config.console.enabled ? &muninn->clogger_q : NULL;
    
    atomic_init(&muninn->gateway.running, false);
    mw_init(&muninn->gateway, "muninn_gateway",
        gateway_loop_fn, gateway_stop_fn, gateway_post_fn,
        (void *)&muninn->gateway_th
    );
    mw_start(&muninn->gateway);

    
    if (config.file.enabled) 
    {
        muninn->flogger_th.reading_queue = &muninn->flogger_q;        
        muninn->flogger_th.output_queue  = config.compressor.enabled ? &muninn->fcompressor_q : NULL; 
        
        atomic_init(&muninn->flogger.running, false);
        mw_init(&muninn->flogger, "muninn_flogger", 
            flogger_loop_fn, flogger_stop_fn, flogger_post_fn,
            (void *)&muninn->flogger_th
        );
        mw_start(&muninn->flogger);
    }

    if (config.console.enabled) 
    {
        muninn->clogger_th.q = &muninn->clogger_q; 
        atomic_init(&muninn->clogger.running, false);
        mw_init(&muninn->clogger, "muninn_clogger",
            clogger_loop_fn, clogger_stop_fn, clogger_post_fn,
            (void *)&muninn->clogger_th
        );
        mw_start(&muninn->clogger);
    }

    if (config.compressor.enabled) 
    {
        muninn->fcompressor_th.q = &muninn->fcompressor_q; 
        atomic_init(&muninn->fcompressor.running, false);
        mw_init(&muninn->fcompressor, "muninn_fcompressor",
            fcompressor_loop_fn, fcompressor_stop_fn, fcompressor_post_fn,
            (void *)&muninn->fcompressor_th
        );
        mw_start(&muninn->fcompressor);
    }

    atomic_init(&muninn->threshold, (char)0);
    atomic_init(&muninn->running, true);

    muninn->mask = MEDM_MESSAGE;

    return true;
}

void muninn_log_internal(muninn_t *m, log_severity_t severity, const char *file, int line, const char *fmt, ...)
{
    if (!atomic_load(&m->running)) return;
    if (severity < atomic_load(&m->threshold)) return;

    va_list args;
    va_start(args, fmt);

    char stack_buffer[1024];
    int req_len = vsnprintf(stack_buffer, sizeof(stack_buffer), fmt, args);
    va_end(args);

    if (req_len < 0) return;

    if ((size_t)req_len + 1 < sizeof(stack_buffer)) 
    {
        stack_buffer[req_len]     = '\n'; //TODO POTENTIAL STACK OVERFLOW ? 
        
        mw_post(&m->gateway,stack_buffer,req_len+1);
    }

    else 
    {
        char   *heap_buffer = malloc(req_len + 1);
        if(heap_buffer == NULL) return;        

        uint8_t *heap_postable = malloc(req_len + 200); //TODO POTENTIAL STACK OVERFLOW
        if(heap_postable == NULL) return;
        
        
        vsnprintf(heap_buffer, req_len + 1, fmt, args);

        size_t encoded_bytes = 0;
        mnn_data_t mnn_message;
        mnn_message.msg_len = strlen(stack_buffer);
        memcpy(mnn_message.msg,stack_buffer,req_len);

        if(mm_encode(&m->mask,&mnn_message,heap_postable,1024,&encoded_bytes) == false ) 
        {
            return; // TODO 1024 
        }
                
        mw_post(&m->gateway,heap_postable,encoded_bytes);
            
        free(heap_buffer);
        free(heap_postable);
        
    }
}

void muninn_shutdown(muninn_t *muninn)
{
    if ( muninn == NULL ) return;

    mw_stop(&muninn->gateway);
    mw_join(&muninn->gateway);

    mw_stop(&muninn->flogger);
    mw_join(&muninn->flogger);

    mw_stop(&muninn->clogger);
    mw_join(&muninn->clogger);

    mw_stop(&muninn->fcompressor);
    mw_join(&muninn->fcompressor);
}

void muninn_set_dynamic_level(muninn_t *muninn, log_severity_t level)
{
    if(muninn == NULL) return;
    atomic_store(&muninn->threshold,(char)level);
}

void muninn_panic_flush(muninn_t *muninn)
{
    if (!atomic_load(&muninn->running)) return; 
    
    execute_panic_flush(muninn);
}
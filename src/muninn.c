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



bool muninn_init(muninn_t *muninn,const char *path)
{
    if(muninn == NULL || path == NULL ) return false;
    memset(muninn,0,sizeof(muninn_t));

    strncpy(muninn->path, path, P_SIZE - 1);
    muninn->path[P_SIZE - 1] = '\0';
    strncpy(muninn->flogger_th.path, path, P_SIZE - 1);
    muninn->flogger_th.path[P_SIZE - 1] = '\0';
    muninn->start_ts = timestamp_u64();

    size_t offset;
    offset = 0;
    for(int i=0;i<LOG_QUEUE_SIZE;i++)
    {
        char *tmp = muninn->flogger_buff + offset;
        setup_queue_message(muninn->flogger_m+i,tmp,LOG_MESSAGE_SIZE);
        offset += LOG_MESSAGE_SIZE;
    }
    ts_queue_setup(&muninn->flogger_q,muninn->flogger_m,LOG_QUEUE_SIZE);

    offset = 0;
    for(int i=0;i<COMP_QUEUE_SIZE;i++)
    {
        char *tmp = muninn->fcompressor_buff + offset;
        setup_queue_message(muninn->fcompressor_m+i,tmp,COMP_MESSAGE_SIZE);
        offset += COMP_MESSAGE_SIZE;
    }
    ts_queue_setup(&muninn->fcompressor_q,muninn->fcompressor_m,COMP_QUEUE_SIZE);

    offset = 0;
    for(int i=0;i<CONS_QUEUE_SIZE;i++)
    {
        char *tmp = muninn->clogger_buff + offset;
        setup_queue_message(muninn->clogger_m+i,tmp,CONS_MESSAGE_SIZE);
        offset += CONS_MESSAGE_SIZE;
    }
    ts_queue_setup(&muninn->clogger_q,muninn->clogger_m,CONS_QUEUE_SIZE);


    ts_rb_setup(&muninn->gateway_rb,(uint8_t *)muninn->gateway_buff,LOG_RB_SIZE);
    muninn->gateway_th.rb = &muninn->gateway_rb;
    muninn->gateway_th.q1 = &muninn->flogger_q;
    muninn->gateway_th.q2 = &muninn->clogger_q;
    atomic_init(&muninn->gateway.running, false);
    mw_init(&muninn->gateway,"muninn_gateway",
        gateway_loop_fn,
        gateway_stop_fn,
        gateway_post_fn,
        (void *)&muninn->gateway_th
    );
    mw_start(&muninn->gateway);


    strcpy(muninn->flogger_th.path,path);
    muninn->flogger_th.reading_queue = &muninn->flogger_q; 
    muninn->flogger_th.output_queue  = &muninn->fcompressor_q; 
    atomic_init(&muninn->flogger.running, false);
    mw_init(&muninn->flogger,"muninn_fcompressor",
        flogger_loop_fn,
        flogger_stop_fn,
        flogger_post_fn,
        (void *)&muninn->flogger_th
    );
    mw_start(&muninn->flogger);

    muninn->clogger_th.q = &muninn->clogger_q; 
    atomic_init(&muninn->clogger.running, false);
    mw_init(&muninn->clogger,"muninn_clogger",
        clogger_loop_fn,
        clogger_stop_fn,
        clogger_post_fn,
        (void *)&muninn->clogger_th
    );
    mw_start(&muninn->clogger);

    muninn->fcompressor_th.q = &muninn->fcompressor_q ; 
    atomic_init(&muninn->fcompressor.running, false);
    mw_init(&muninn->fcompressor,"muninn_fcompressor",
        fcompressor_loop_fn,
        fcompressor_stop_fn,
        fcompressor_post_fn,
        (void *)&muninn->fcompressor_th
    );
    mw_start(&muninn->fcompressor);

    atomic_init(&muninn->threshold, (char)0);
    atomic_init(&muninn->running, true);

    return true;
}

void muninn_log_internal(muninn_t *m, log_severity_t severity, const char *file, int line, const char *fmt, ...)
{
    if (!atomic_load(&m->running)) return;

    if ( severity < atomic_load(&m->threshold) ) return;

    va_list args, args_copy;
    va_start(args, fmt);
    
    va_copy(args_copy, args);

    char stack_buffer[1024];
    int req_len = vsnprintf(stack_buffer, sizeof(stack_buffer), fmt, args);
    va_end(args);

    if (req_len < 0) {
        va_end(args_copy);
        return; 
    }

    if ((size_t)req_len < sizeof(stack_buffer)) 
    {
        // stack buffer was enough. 
        // send it to the logger thread
        mw_post(&m->gateway,stack_buffer);
    } 
    else 
    {
        //using malloc for bigger lines
        char *heap_buffer = malloc(req_len + 1);
        
        if (heap_buffer != NULL) 
        {
            vsnprintf(heap_buffer, req_len + 1, fmt, args_copy);
            
            mw_post(&m->gateway,heap_buffer);
            
            free(heap_buffer);
        }
    }

    va_end(args_copy);
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
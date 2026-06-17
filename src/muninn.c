#include <muninn.h>

#include "internal/muninn_int.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <internal/timestamp_gen.h>
#include <internal/compressor_th.h>
#include <internal/logger_th.h>
#include <internal/panic_flusher.h>

#include <internal/gateway_th.h>



bool muninn_init(muninn_t *muninn,const char *path)
{
    if(muninn == NULL || path == NULL ) return false;
    memset(muninn,0,sizeof(muninn_t));

    strncpy(muninn->path, path, P_SIZE - 1);
    muninn->path[P_SIZE - 1] = '\0';
    strncpy(muninn->logger_th.path, path, P_SIZE - 1);
    muninn->logger_th.path[P_SIZE - 1] = '\0';
    muninn->start_ts = timestamp_u64();

    ts_rb_setup(&muninn->logger_rb,(uint8_t *)muninn->buffer_logger,LOG_RB_SIZE);

    size_t offset;
    offset = 0;
    for(int i=0;i<COMP_QUEUE_SIZE;i++)
    {
        char *tmp = muninn->buffer_compressor + offset;
        setup_queue_message(muninn->queue_compressor+i,tmp,COMP_MESSAGE_SIZE);
        offset += COMP_MESSAGE_SIZE;
    }
    ts_queue_setup(&muninn->compressor_q,muninn->queue_compressor,COMP_QUEUE_SIZE);

    offset = 0;
    for(int i=0;i<CONS_QUEUE_SIZE;i++)
    {
        char *tmp = muninn->buffer_console + offset;
        setup_queue_message(muninn->queue_console+i,tmp,CONS_MESSAGE_SIZE);
        offset += COMP_MESSAGE_SIZE;
    }
    ts_queue_setup(&muninn->console_q,muninn->queue_console,COMP_QUEUE_SIZE);


    muninn->logger_th.ringbuffer  = &muninn->logger_rb;
    muninn->logger_th.compress_q  = &muninn->compressor_q;

    muninn->console_th.tasks      = &muninn->console_q;
       
    muninn->compressor_th.tasks   = &muninn->compressor_q;
    
    logger_th_start(&muninn->logger_th);
    console_th_start(&muninn->console_th);
    compressor_th_start(&muninn->compressor_th);
    
    atomic_init(&muninn->threshold, (char)0);
    atomic_init(&muninn->running, true);

    mw_init(&muninn->gateway,"muninn_gateway",gateway_loop_fn,gateway_stop_fn,(void *)&muninn);

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
        logger_th_perform(&m->logger_th, severity, stack_buffer);
    } 
    else 
    {
        //using malloc for bigger lines
        char *heap_buffer = malloc(req_len + 1);
        
        if (heap_buffer != NULL) 
        {
            vsnprintf(heap_buffer, req_len + 1, fmt, args_copy);
            
            logger_th_perform(&m->logger_th, severity, heap_buffer);
            
            free(heap_buffer);
        }
    }

    va_end(args_copy);
}

void muninn_shutdown(muninn_t *muninn)
{
    if ( muninn == NULL ) return;

    console_th_stop(&muninn->console_th);
    console_th_join(&muninn->console_th);

    logger_th_stop(&muninn->logger_th);
    logger_th_join(&muninn->logger_th);

    compressor_th_stop(&muninn->compressor_th);
    compressor_th_join(&muninn->compressor_th);

    mw_stop(&muninn->gateway);
    mw_join(&muninn->gateway);
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
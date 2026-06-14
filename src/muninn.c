#include "internal/munin_int.h"
#include <string.h>

#include <internal/timestamp_gen.h>
#include <internal/compressor_th.h>
#include <internal/logger_th.h>

#include "muninn.h"

bool muninn_init(muninn_t *muninn,const char *path)
{
    if(muninn == NULL || path == NULL ) return false;
    memset(muninn,0,sizeof(muninn));

    strncpy(muninn->path, path, P_SIZE - 1);
    muninn->path[P_SIZE - 1] = '\0';
    strncpy(muninn->logger_th.path, path, P_SIZE - 1);
    muninn->logger_th.path[P_SIZE - 1] = '\0';

    ts_rb_setup(&muninn->logger_rb,(uint8_t *)muninn->buffer_logger,LOG_RB_SIZE);

    size_t offset = 0;
    for(int i=0;i<COMP_QUEUE_SIZE;i++)
    {
        char *tmp = muninn->buffer_compressor + offset;
        setup_queue_message(muninn->queue_compressor+i,tmp,COMP_MESSAGE_SIZE);
        offset += COMP_MESSAGE_SIZE;
    }
    ts_queue_setup(&muninn->compressor_q,muninn->queue_compressor,COMP_QUEUE_SIZE);


    muninn->logger_th.ringbuffer = &muninn->logger_rb;
    muninn->logger_th.compress_q = &muninn->compressor_q;
       
    muninn->compressor_th.tasks = &muninn->compressor_q;
    
    logger_th_start(&muninn->logger_th);
    compressor_th_start(&muninn->compressor_th);
    atomic_init(&muninn->running, true);

    muninn->start_ts = timestamp_u64();

    return true;
}

bool muninn_log_dbg(muninn_t *muninn,const char *msg)
{
    if ( muninn == NULL || msg == NULL ) return false;
    logger_th_perform(&muninn->logger_th,LOG_DEBUG,msg);
    return true;
}
bool muninn_log_info(muninn_t *muninn,const char *msg)
{
    if ( muninn == NULL || msg == NULL ) return false;
    logger_th_perform(&muninn->logger_th,LOG_INFO,msg);
    return true;
}
bool muninn_log_warn(muninn_t *muninn,const char *msg)
{
    if ( muninn == NULL || msg == NULL ) return false;
    logger_th_perform(&muninn->logger_th,LOG_WARN,msg);
    return true;
}
bool muninn_log_error(muninn_t *muninn,const char *msg)
{
    if ( muninn == NULL || msg == NULL ) return false;
    logger_th_perform(&muninn->logger_th,LOG_ERROR,msg);
    return true;
}
bool muninn_log_fatal(muninn_t *muninn,const char *msg)
{
    if ( muninn == NULL || msg == NULL ) return false;
    logger_th_perform(&muninn->logger_th,LOG_FATAL,msg);
    return true;
}

void muninn_shutdown(muninn_t *muninn)
{
    if ( muninn == NULL ) return;
    logger_th_stop(&muninn->logger_th);
    logger_th_join(&muninn->logger_th);

    compressor_th_stop(&muninn->compressor_th);
    compressor_th_join(&muninn->compressor_th);
}

#include "internal/munin_int.h"
#include <string.h>

#include <internal/compressor_th.h>
#include <internal/logger_th.h>

#include "muninn.h"

void muninn_init(muninn_t *muninn,const char *path)
{
    strncpy(muninn->path, path, P_SIZE - 1);
    muninn->path[P_SIZE - 1] = '\0';
    strncpy(muninn->logger_th.path, path, P_SIZE - 1);
    muninn->logger_th.path[P_SIZE - 1] = '\0';

    size_t offset = 0;
    for(int i=0;i<LOG_QUEUE_SIZE;i++)
    {
        char *buffer_i = muninn->buffer_logger + offset;
        queue_message_t *message_i = &muninn->queue_logger[0]+i;
        setup_queue_message(message_i,buffer_i,LOG_MESSAGE_SIZE);
        offset += LOG_MESSAGE_SIZE;
    }
    ts_queue_setup(&muninn->logger_q,muninn->queue_logger,LOG_QUEUE_SIZE);

    offset = 0;
    for(int i=0;i<COMP_QUEUE_SIZE;i++)
    {
        char *tmp = muninn->buffer_compressor + offset;
        setup_queue_message(muninn->queue_compressor+i,tmp,COMP_MESSAGE_SIZE);
        offset += COMP_MESSAGE_SIZE;
    }
    ts_queue_setup(&muninn->compressor_q,muninn->queue_compressor,COMP_QUEUE_SIZE);


    muninn->logger_th.queue = &muninn->logger_q;
    muninn->logger_th.compress_q = &muninn->compressor_q;
       
    muninn->compressor_th.tasks = &muninn->compressor_q;
    
    logger_th_start(&muninn->logger_th);
    compressor_th_start(&muninn->compressor_th);
    atomic_init(&muninn->running, true);
}

void muninn_log_dbg(muninn_t *muninn,const char *msg)
{
    logger_th_perform(&muninn->logger_th,LOG_DEBUG,msg);
}
void muninn_log_info(muninn_t *muninn,const char *msg)
{
    logger_th_perform(&muninn->logger_th,LOG_INFO,msg);
}
void muninn_log_warn(muninn_t *muninn,const char *msg)
{
    logger_th_perform(&muninn->logger_th,LOG_WARN,msg);
}
void muninn_log_error(muninn_t *muninn,const char *msg)
{
    logger_th_perform(&muninn->logger_th,LOG_ERROR,msg);
}
void muninn_log_fatal(muninn_t *muninn,const char *msg)
{
    logger_th_perform(&muninn->logger_th,LOG_FATAL,msg);
}

void muninn_shutdown(muninn_t *muninn)
{
    compressor_th_stop(&muninn->compressor_th);
    compressor_th_join(&muninn->compressor_th);

    logger_th_stop(&muninn->logger_th);
    logger_th_join(&muninn->logger_th);
}

#include "internal/munin_int.h"
#include <string.h>

#include <internal/compressor_th.h>
#include <internal/logger_th.h>

#include "muninn.h"

void muninn_init(const char *path, muninn_t *muninn)
{
    strncpy(muninn->path, path, P_SIZE - 1);

    setup_queue_message(
        muninn->logger_q.queue.messages,
        muninn->buffer_logger,
        M_SIZE);   
    ts_queue_setup(
        &muninn->logger_q,
        muninn->logger_q.queue.messages,
        Q_SIZE);

    setup_queue_message(
        muninn->compressor_q.queue.messages,
        muninn->buffer_logger,
        M_SIZE);   
    ts_queue_setup(
        &muninn->compressor_q,
        muninn->compressor_q.queue.messages,
        Q_SIZE);

    strncpy(muninn->logger_th.path, path, P_SIZE - 1);
    muninn->logger_th.queue = &muninn->logger_q;
    muninn->logger_th.compress_q = &muninn->compressor_q;
       
    muninn->compressor_th.tasks = &muninn->compressor_q;
    
    logger_th_start(&muninn->logger_th);
    compressor_th_start(&muninn->compressor_th);
    atomic_init(&muninn->running, true);
}

void muninn_log(muninn_t *muninn, const char *msg)
{
    logger_th_perform(&muninn->logger_th,msg);
}

void muninn_shutdown(muninn_t *muninn)
{
    compressor_th_stop(&muninn->compressor_th);
    compressor_th_join(&muninn->compressor_th);

    logger_th_stop(&muninn->logger_th);
    logger_th_join(&muninn->logger_th);
}

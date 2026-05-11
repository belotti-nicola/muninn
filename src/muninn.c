#include "internal/munin_int.h"
#include <string.h>
#include <internal/zstd_wrapper.h>

#include <internal/compressor_th.h>
#include <internal/logger_th.h>

#include "muninn.h"

void muninn_init(const char *path, muninn_t *muninn)
{
    ts_queue_setup(&muninn->logger_q);
    ts_queue_setup(&muninn->compressor_q);
    
    logger_th_data lthdata;
    lthdata.queue = muninn->logger_q;
    strncpy(lthdata.path, path, P_SIZE - 1);
    lthdata.written_bytes = 0;
    lthdata.file = NULL;

    compressor_th_data cthdata;
    cthdata.tasks = muninn->compressor_q;
    
    logger_th_start(&lthdata);
    compressor_th_start(&cthdata);

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

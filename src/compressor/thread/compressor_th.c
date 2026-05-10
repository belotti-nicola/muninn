#include <internal/zstd_wrapper.h>

#include "compressor_th.h"

void *compressor_thread_function(void *arg)
{
    ts_queue_t *q =(ts_queue_t *)arg;
    
    char message[Q_SIZE];
    bool rc;
    while(true)
    {
        rc = ts_queue_pop(q,message);
        if ( rc == false )
        {
            break;
        }

        if ( strlen(message) > 0)
        {
            zstd_compress_file(message);
        }
    }
    
    return NULL;
}

void compressor_thread_start(compressor_th_data *compressor_th)
{
    pthread_t th;
    int rc = pthread_create(&th,NULL,compressor_thread_function,NULL);
    if (rc != 0)
    {
        exit(-1);
    }
}
void compressor_thread_stop(compressor_th_data *compressor_th)
{
    ts_queue_stop(&compressor_th->q);
    int rc = pthread_join(&compressor_th->th);
    if (rc != 0)
    {
        exit(-1);
    }
}

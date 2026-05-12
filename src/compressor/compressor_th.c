#include <string.h>
#include <internal/zstd_wrapper.h>

#include <internal/compressor_th.h>

int compressor_th_start(compressor_th_data *cth_data)
{
    pthread_t th;
    int rc = pthread_create(&th,NULL,compressor_th_function,cth_data);
    if(rc != 0)
    {
        fprintf(stderr,"Error starting thread.\n");
        return 1;
    }
    cth_data->th = th;
    return 0;
}
int compressor_th_stop(compressor_th_data *cth_data)
{
    ts_queue_stop(cth_data->tasks);
    return 0;
}

int compressor_th_join(compressor_th_data *cth_data)
{
    int rc = pthread_join(cth_data->th,NULL);
    if(rc != 0)
    {
        fprintf(stderr,"Error joining compressor thread.\n");
        return 1;
    }

    return 0;
}

static void *compressor_th_function(void *arg)
{
    compressor_th_data *cth_data = (compressor_th_data *)arg;
    
    atomic_store(&cth_data->running,true);

    queue_message_t qm;
    while(true)
    {
        if(ts_queue_pop(cth_data->tasks,&qm))
        {
            int rc = zstd_compress_file(qm.message);
            printf("zstd_compress_file %d\n",rc);
        }
        else 
        {
            break;
        }

    }
    fprintf(stdout,"Compressor end.\n");
    atomic_store(&cth_data->running,false);
    return NULL;
}

void compressor_th_perform(compressor_th_data *cth_data,const char *filepath)
{   
    queue_message_t qm; 
    strcpy(qm.message,filepath);
    ts_queue_push(cth_data->tasks,&qm);
}
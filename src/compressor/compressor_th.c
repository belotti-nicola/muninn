#include <string.h>

#include <internal/compressor_th.h>
#include <internal/lz4_wrapper.h>
#include <time.h>


#define LZ4_SUFFIX ".lz4"
#define R_PATH_SIZE P_SIZE + strlen(LZ4_SUFFIX)

void create_new_file_name(const char *in, char *out, size_t out_size) 
{ 
    char ts[32];
    time_t now = time(NULL);
    strftime(ts, sizeof(ts), "_%Y%m%d_%H%M%S", localtime(&now));

    const char *dot = strrchr(in, '.');

    if(dot && dot != in)
    {
        size_t base_len = dot - in;
        snprintf(out, out_size, "%.*s%s%s", (int)base_len, in, ts, LZ4_SUFFIX);
    } 
    else
    {
        snprintf(out, out_size, "%s%s%s", in, ts, LZ4_SUFFIX);
    }
}


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
            char lz4_file_name[R_PATH_SIZE]; memset(lz4_file_name,0,R_PATH_SIZE);
            create_new_file_name(qm.message,lz4_file_name,R_PATH_SIZE);
            int rc = lz4_compress_file(qm.message,lz4_file_name,3);
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
#include <string.h>

#include <internal/compressor_th.h>
#include <internal/lz4_wrapper.h>
#include <time.h>
#include <unistd.h>


#define ROTATING_SUFFIX ".rotating"
#define LZ4_SUFFIX ".lz4"

void create_timestamp(char *buf, size_t size)
{
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);

    snprintf(buf,
             size,
             "%lld_%09ld",
             (long long)ts.tv_sec,
             ts.tv_nsec);
}


void create_compressed_file_name(const char *in,
                                 char *out,
                                 size_t out_size)
{   
    const char *rot = strstr(in, ROTATING_SUFFIX);
    size_t base_len;

    if (rot)
    {
        base_len = (size_t)(rot - in);
    }
    else
    {
        base_len = strlen(in);
    }

    size_t suffix_len = strlen(LZ4_SUFFIX);

    if (out_size <= suffix_len + 1)
    {
        if (out_size > 0)
            out[0] = '\0';
        return;
    }

    size_t max_base_len = out_size - suffix_len - 1;

    if (base_len > max_base_len)
        base_len = max_base_len;

    
    char ts[32];create_timestamp(ts,32);
    snprintf(out,
             out_size,
             "%.*s_%s%s",
             (int)base_len,
             in,
             ts,
             LZ4_SUFFIX);
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

    char qm[1024];
    while(true)
    {
        if(ts_queue_pop(cth_data->tasks,qm))
        {
            char lz4_file_name[P_SIZE]; memset(lz4_file_name,0,P_SIZE);
            create_compressed_file_name(qm,lz4_file_name,P_SIZE);
            int rc = lz4_compress_file(qm,lz4_file_name,3);
            if (rc != 0)
            {
                fprintf(stderr,"Error: lz4_compress_file returned %d.\n",rc);
            }
            else 
            {
                printf("Compressed: %s->%s\n",qm,lz4_file_name);
            }
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
    ts_queue_push(cth_data->tasks,filepath);
}
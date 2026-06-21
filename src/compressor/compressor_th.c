#include <string.h>

#include <internal/compressor_th.h>
#include <internal/lz4_wrapper.h>
#include <time.h>
#include <unistd.h>

#include <muninn.h> 


#define ROTATING_SUFFIX ".rotating"
#define LZ4_SUFFIX ".lz4"

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

    
    snprintf(out,
             out_size,
             "%.*s%s",
             (int)base_len,
             in,
             LZ4_SUFFIX);
}



void *fcompressor_stop_fn(void *arg)
{
    if( arg == NULL ) return NULL;
    compressor_th_data *cth_data = (compressor_th_data *)arg;

    ts_queue_stop(cth_data->q);
    return NULL;
}

void *fcompressor_loop_fn(void *arg)
{
    if( arg == NULL ) return NULL;
    compressor_th_data *cth_data = (compressor_th_data *)arg;
    
    char buffer[P_SIZE] = {0};
    queue_message_t qm = {0};
    setup_queue_message(&qm,buffer,P_SIZE);
    while(true)
    {
        if(ts_queue_pop(cth_data->q,&qm))
        {
            char lz4_file_name[P_SIZE]; memset(lz4_file_name,0,P_SIZE);
            create_compressed_file_name(qm.data,lz4_file_name,P_SIZE);
            int rc = lz4_compress_file(qm.data,lz4_file_name,3);
            if (rc != 0)
            {
                fprintf(stderr,"Error: lz4_compress_file returned %d.\n",rc);
            }
            else 
            {
                printf("Compressed: %s->%s\n",qm.data,lz4_file_name);
                remove(qm.data);
            }
        }
        else 
        {
            break;
        }

    }
    fprintf(stdout,"Compressor end.\n");
    return NULL;
}

void *fcompressor_post_fn(void *context,void *arg)
{   
    if( arg == NULL || context == NULL ) return NULL;
    compressor_th_data *cth_data = (compressor_th_data *)context;

    ts_queue_push(cth_data->q,1,arg);

    return NULL;
}
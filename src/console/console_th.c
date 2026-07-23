#include <internal/clogger_th.h>
#include <muninn.h>
#include <string.h>

#define BUFFSIZE 4096

void *clogger_stop_fn(void *arg)
{
    if(arg == NULL) return NULL;

    clogger_th_data *cth_data = (clogger_th_data *)arg;
    
    ts_queue_stop(cth_data->q);
    
    return NULL;
}

void *clogger_post_fn(void *context, void *arg, size_t size)
{
    if(context == NULL || arg == NULL) return NULL;

    clogger_th_data *cth_data = (clogger_th_data *)context;
    const char      *message  = (const char      *)arg;

    ts_queue_n_push(cth_data->q,1,message,size);

    return NULL;
}

void *clogger_loop_fn(void *arg)
{
    if(arg == NULL) return NULL;

    clogger_th_data *cth_data = (clogger_th_data *)arg;
    
    char data[P_SIZE] = {0};
    queue_message_t console_m = {0};
    setup_queue_message(&console_m,data,P_SIZE);

    char buffer[BUFFSIZE]={0};
    size_t buffer_size = 0;
    while(ts_queue_pop(cth_data->q,&console_m))
    {
        if(console_m.size == 0)
        {
            continue;
        }

        if(buffer_size + console_m.size > BUFFSIZE)
        {
            continue;
        }

        strncat(buffer,console_m.data,console_m.size);
        buffer_size += console_m.size;

        if(buffer_size < BUFFSIZE * 0.95)
        {
            continue;
        }

        console_handler(
            &cth_data->ch,
            1,//todo
            buffer,
            buffer_size
        );

        buffer_size = 0;
        buffer[0] = '\0';
    }


    if(buffer_size > 0)
    {
        console_handler(
            &cth_data->ch,
            1,//todo
            buffer,
            buffer_size
        );
    }
    printf("Clogger end\n");
    return NULL;
}
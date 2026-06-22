#include <internal/clogger_th.h>
#include <muninn.h>


void *clogger_stop_fn(void *arg)
{
    if(arg == NULL) return NULL;

    clogger_th_data *cth_data = (clogger_th_data *)arg;
    
    ts_queue_stop(cth_data->q);
    
    return NULL;
}

void *clogger_post_fn(void *context, void *arg)
{
    if(context == NULL || arg == NULL) return NULL;

    clogger_th_data *cth_data = (clogger_th_data *)context;
    const char      *message  = (const char      *)arg;

    ts_queue_push(cth_data->q,1,message);

    return NULL;
}

void *clogger_loop_fn(void *arg)
{
    if(arg == NULL) return NULL;

    clogger_th_data *cth_data = (clogger_th_data *)arg;
    
    char buffer[P_SIZE] = {0};
    queue_message_t console_m = {0};
    setup_queue_message(&console_m,buffer,P_SIZE);

    while(ts_queue_pop(cth_data->q,&console_m))
    {
        if(console_m.size == 0)
        {
            continue;
        }

        console_handler(
            &cth_data->ch,
            1,//todo
            console_m.data,
            console_m.size
        );
    }

    printf("Clogger end\n");
    return NULL;
}
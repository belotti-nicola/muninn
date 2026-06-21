#include <internal/clogger_th.h>
#include <muninn.h>


void *clogger_stop_fn(void *arg)
{
    if(arg == NULL) return NULL;

    clogger_th_data *cth_data = (clogger_th_data *)arg;
    
    muninn_t *muninn = cth_data->muninn;
    if(muninn == NULL) return NULL;

    ts_queue_stop(&muninn->clogger_q);
    
    return NULL;
}

void *clogger_post_fn(void *context, void *arg)
{
    if(context == NULL || arg == NULL) return NULL;

    clogger_th_data *cth_data = (clogger_th_data *)context;
    const char      *message  = (const char      *)arg;

    muninn_t *muninn = cth_data->muninn;
    if(muninn == NULL) return NULL;

    ts_queue_push(&muninn->clogger_q,1,message);

    return NULL;
}

void *clogger_loop_fn(void *arg)
{
    if(arg == NULL) return NULL;

    clogger_th_data *cth_data = (clogger_th_data *)arg;

    muninn_t *muninn = cth_data->muninn;
    if(muninn == NULL) return NULL;
    
    char buffer[P_SIZE] = {0};
    queue_message_t console_m = {0};
    setup_queue_message(&console_m,buffer,P_SIZE);

    while(ts_queue_pop(&muninn->clogger_q,&console_m))
    {
        if(console_m.size == 0)
        {
            continue;
        }

        console_handler(
            &cth_data->ch,
            console_m.severity,
            console_m.data,
            console_m.size
        );
    }

    printf("Clogger end\n");
    return NULL;
}
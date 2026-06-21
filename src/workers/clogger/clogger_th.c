#include <internal/clogger_th.h>

#include <muninn.h>

void *clogger_stop_fn(void *arg)
{
    if(arg == NULL) return NULL;
    clogger_th_data *cth_data = (clogger_th_data *)arg;

    if(&cth_data->muninn->clogger_q == NULL)
    {
        fprintf(stderr,"Error stopping thread.\n");
        return NULL;
    }
    ts_queue_stop(&cth_data->muninn->clogger_q);
    
    return NULL;
}

void *clogger_join_fn(clogger_th_data *cth_data)
{
    if(cth_data == NULL || &cth_data->muninn->clogger_q == NULL)
    {
        fprintf(stderr,"Error trying to join thread.\n");
        return NULL;
    }

    return NULL;
}
void *clogger_post_fn(clogger_th_data *cth_data,const char *message)
{
    if(cth_data == NULL || &cth_data->muninn->clogger_q == NULL)
    {
        fprintf(stderr,"Error trying to join thread.\n");
        return NULL;
    }

    ts_queue_push(&cth_data->muninn->clogger_q == NULL,1,message);
    return NULL;
}

void *clogger_post_fn(void *arg)
{
    if(arg == NULL) return NULL;

    clogger_th_data *cth_data = (clogger_th_data *)arg;
    
    char buffer[P_SIZE] = {0};
    queue_message_t console_m = {0};
    setup_queue_message(&console_m,buffer,P_SIZE);

    while(ts_queue_pop(&cth_data->muninn->clogger_q,&cth_data->muninn->clogger_m))
    {
        if(console_m.size == 0)
        {
            continue;
        }

        console_handler(
            &cth_data,
            console_m.severity,
            console_m.data,
            console_m.size
        );
    }

    printf("Console logger end\n");
    return NULL;
}
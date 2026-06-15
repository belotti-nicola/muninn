#include <internal/console_th.h>

int console_th_start(console_th_data *cth_data)
{
    console_handler_setup(&cth_data->ch);
    
    int rc = pthread_create(&cth_data->th,NULL,console_th_function,cth_data);
    if(rc != 0)
    {
        fprintf(stderr,"Error starting thread.\n");
        return 1;
    }
    
    return 0;
}
int console_th_stop(console_th_data *cth_data)
{
    if(cth_data == NULL || cth_data->tasks == NULL)
    {
        fprintf(stderr,"Error stopping thread.\n");
        return 1;
    }
    ts_queue_stop(cth_data->tasks);
    
    return 0;
}
int console_th_join(console_th_data *cth_data)
{
    if(cth_data == NULL || cth_data->tasks == NULL)
    {
        fprintf(stderr,"Error trying to join thread.\n");
        return 1;
    }

    int rc = pthread_join(cth_data->th,NULL);
    if(rc != 0)
    {
        fprintf(stderr,"Error joining compressor thread.\n");
        return 1;
    }

    return 0;
}
void console_th_perform(console_th_data *cth_data,log_severity_t severity, const char *message)
{
    if(cth_data == NULL || cth_data->tasks == NULL)
    {
        fprintf(stderr,"Error trying to join thread.\n");
        return;
    }

    ts_queue_push(cth_data->tasks,severity,message);
}

static void *console_th_function(void *arg)
{
    console_th_data *cth_data = (console_th_data *)arg;
    
    char buffer[P_SIZE] = {0};
    queue_message_t console_m = {0};
    setup_queue_message(&console_m,buffer,P_SIZE);

    atomic_store(&cth_data->running,true);
    while(ts_queue_pop(cth_data->tasks,&console_m))
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
    atomic_store(&cth_data->running,false);

    return NULL;
}
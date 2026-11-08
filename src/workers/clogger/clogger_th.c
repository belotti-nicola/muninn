#include <internal/clogger_th.h>
#include <internal/muninn_message.h>
#include <internal/protocols/muninn_encoded_messages/muninn_encoded_message_codec.h>

#include <muninn.h>


void *clogger_loop_fn(void *arg)
{
    if(arg == NULL) return NULL;

    clogger_th_data *cth_data = (clogger_th_data *)arg;
    
    char queu_buffer[P_SIZE] = {0};
    queue_message_t console_m = {0};
    setup_queue_message(&console_m,queu_buffer,P_SIZE);

    const char *pattern = "<msg>";
    size_t pattern_size = 5;
    if ( muninn_formatter_init(&cth_data->formatter,pattern,pattern_size) == false )
    {
        printf("muninn_formatter_init error\n");
        return NULL;
    }

    char file_buffer[256] = {0};
    char function_buffer[256] = {0};
    char msg_buffer[256] = {0};

    muninn_message_t mm;
    mm.file = file_buffer;
    mm.func = function_buffer;
    mm.msg = msg_buffer;

    uint8_t buffer[1024] = {0};
    size_t buffer_size;

    char expanded[2048] = {0};
    size_t expanded_size = 2048;

    while( ts_queue_n_pop(cth_data->q,buffer,1024,&buffer_size) )
    {
        if(buffer_size == 0)
        {
            continue;
        }

        if ( muninn_messages_decode(buffer,buffer_size,&mm) == false)
        {
            continue;
        }

        if ( muninn_formatter_expand(&cth_data->formatter,mm,expanded,&expanded_size,2048) == false )
        {
            continue;
        }

        console_handler(
            &cth_data->ch,
            mm.severity,
            mm.msg,
            mm.msg_len
        );
    }

    printf("Console logger end\n");
    return NULL;
}

void *clogger_stop_fn(void *arg)
{
    if(arg == NULL) return NULL;
    clogger_th_data *cth_data = (clogger_th_data *)arg;

    if(cth_data->q == NULL)
    {
        fprintf(stderr,"Error stopping thread.\n");
        return NULL;
    }
    ts_queue_stop(cth_data->q);
    
    return NULL;
}

void *clogger_join_fn(clogger_th_data *cth_data)
{
    if(cth_data == NULL || &cth_data->q == NULL)
    {
        fprintf(stderr,"Error trying to join thread.\n");
        return NULL;
    }

    return NULL;
}

void *clogger_post_fn(void *context, void *data, size_t data_size)
{
    if(context == NULL || data == NULL)
    {
        fprintf(stderr,"Error trying to join thread.\n");
        return NULL;
    }

    clogger_th_data *cth_data = (clogger_th_data *)context;
    uint8_t *message = (uint8_t *)data;

    ts_queue_n_push(cth_data->q,message,data_size);
    return NULL;
}

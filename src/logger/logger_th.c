#include <string.h>

#include <internal/logger_th.h>

int logger_th_start(logger_th_data *lth_data)
{
    pthread_t th;
    int rc = pthread_create(&th,NULL,logger_th_function,lth_data);
    if(rc != 0)
    {
        fprintf(stderr,"Error starting thread.\n");
        return 1;
    }
    lth_data->th = th;
    return 0;
}

int logger_th_stop(logger_th_data *lth_data)
{
    ts_queue_stop(lth_data->queue);
    return 0;
}

int logger_th_join(logger_th_data *lth_data)
{  
    int rc = pthread_join(lth_data->th, NULL);
    if( rc != 0 )
    {
        fprintf(stderr,"Error joining logger thread.\n");
        return 1;
    }
    ts_queue_release(lth_data->queue);
    
    return 0;
}

void logger_th_perform(logger_th_data *lth_data,const char *message)
{
    queue_message_t qm;
    strncpy(qm.message, message, sizeof(qm.message) - 1);
    qm.message[sizeof(qm.message) - 1] = '\0';

    ts_queue_push(lth_data->queue, &qm);
}

static void *logger_th_function(void *arg)
{
    logger_th_data *lth = (logger_th_data *)arg;

    lth->file = fopen(lth->path, "a");
    if (!lth->file)
    {
        fprintf(stderr,"Error: could not open file '%s'",lth->path);
        return NULL;
    }

    atomic_store(&lth->running, true);
    queue_message_t qm;
    queue_message_t qm_compressor;
    strcpy(qm.message,lth->path);

    while (ts_queue_pop(lth->queue, &qm))
    {
        fprintf(lth->file, "%s\n", qm.message);
        fflush(lth->file);
        lth->written_bytes += strlen(qm.message)+1;
        if(lth->written_bytes < F_MAX_SIZE)
        {
            continue;
        }
        fclose(lth->file);
        
        char rotating_file[256];
        snprintf(rotating_file, sizeof(rotating_file), "%s", lth->path);
        char *dot = strrchr(rotating_file, '.');
        if(dot)
        {
            strcpy(dot, ".rotating");
        }
        else
        {
            strcat(rotating_file, ".rotating");
        }
        strcpy(qm_compressor.message,rotating_file);
        rename(lth->path,rotating_file);

        ts_queue_push(lth->compress_q,&qm_compressor);
        lth->file = fopen(lth->path, "a");
        if (!lth->file)
        {
            return NULL;
        }
        lth->written_bytes = 0;
    }
    
    atomic_store(&lth->running, false);
    fclose(lth->file);
    lth->file = NULL;
    fprintf(stdout,"Logger end.\n");
    return NULL;
}
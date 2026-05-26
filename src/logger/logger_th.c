#include <string.h>
#include <unistd.h>

#include <internal/logger_th.h>

#define TS_SIZE 16
#define ROTATING_SUFFIX ".rotating"


void create_rotate_file_name(const char *in, char *out, size_t out_size)
{
    char ts[TS_SIZE];

    time_t now = time(NULL);
    snprintf(ts, sizeof(ts), "%ld", (long)now);

    const char *dot = strrchr(in, '.');

    size_t suffix_len =
        1 +                     // "_"
        strlen(ts) +
        strlen(ROTATING_SUFFIX);

    // spazio disponibile per il nome base
    size_t max_base_len;

    if (out_size <= suffix_len + 1)
    {
        // buffer troppo piccolo
        if (out_size > 0)
            out[0] = '\0';
        return;
    }

    max_base_len = out_size - suffix_len - 1;

    size_t base_len;

    if (dot && dot != in)
        base_len = (size_t)(dot - in);
    else
        base_len = strlen(in);

    if (base_len > max_base_len)
        base_len = max_base_len;

    snprintf(out,
             out_size,
             "%.*s_%s%s",
             (int)base_len,
             in,
             ts,
             ROTATING_SUFFIX);
}


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

void logger_th_perform(logger_th_data *lth_data,log_severity_t severity, const char *message)
{
    ts_queue_push(lth_data->queue,severity, message);
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
    char qm[LOG_MESSAGE_SIZE] = {0};
    char qm_compressor[P_SIZE] = {0};

    queue_message_t message = {0};
    setup_queue_message(&message,qm,LOG_MESSAGE_SIZE);
    while (ts_queue_pop(lth->queue,&message))
    {
        const char *sev_str = "UNKN";
        switch (message.severity) {
            case LOG_DEBUG: sev_str = "DEBUG"; break;
            case LOG_INFO:  sev_str = "INFO "; break;
            case LOG_WARN:  sev_str = "WARN "; break;
            case LOG_ERROR: sev_str = "ERROR"; break;
            case LOG_FATAL: sev_str = "FATAL"; break;
            case LOG_NONE:  sev_str = "     "; break; 
        }
        
        int written = fprintf(lth->file, "[%s] %.*s\n", sev_str, (int)message.size, message.data);
        if (written > 0)
        {
            lth->written_bytes += written;
        }
        if (message.severity >= LOG_ERROR) 
        {
            fdatasync(fileno(lth->file)); 
        }
        if(lth->written_bytes < F_MAX_SIZE && message.severity <=LOG_WARN)
        {
            continue;
        }    
        
        fflush(lth->file);
        
        if(lth->written_bytes < F_MAX_SIZE)
        {
            continue;
        }
        fclose(lth->file);
        
        char rotating_file[P_SIZE];memset(rotating_file,0,P_SIZE);
        create_rotate_file_name(lth->path,rotating_file,P_SIZE);
        int rc = rename(lth->path,rotating_file);
        if( rc != 0)
        {
            fprintf(stderr,"Error: could not rename %s in %s.\n",lth->path,rotating_file);
            return NULL;
        }

        strncpy(qm_compressor,rotating_file,sizeof(qm_compressor) - 1);
        qm_compressor[sizeof(qm_compressor) - 1] = '\0';
        ts_queue_push(lth->compress_q, LOG_NONE,qm_compressor);
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
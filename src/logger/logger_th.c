#include <string.h>
#include <unistd.h>

#include <internal/logger_th.h>
#include <internal/ts_rb_message.h>
#include <internal/timestamp_gen.h>
#include <stdlib.h>

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
    ts_rb_stop(lth_data->ringbuffer);
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
    ts_rb_release(lth_data->ringbuffer);
    
    return 0;
}

bool logger_th_perform(logger_th_data *lth_data,log_severity_t severity, const char *message_content)
{
    if(lth_data == NULL || message_content == NULL) return false;

    ts_rb_message_t message = {0};
    size_t full_len = strlen(message_content) + sizeof(ts_rb_header_t);
    uint64_t ts = timestamp_u64();
    uint8_t sev = (uint8_t)severity;
    ts_rb_message_set(&message,full_len,ts,sev,(uint8_t *)message_content);

    return ts_rb_push(lth_data->ringbuffer,&message);
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
    char qm[1024] = {0};//TODO
    char qm_compressor[P_SIZE] = {0};

    ts_rb_message_t message = {0};
    ts_rb_message_setup(&message,qm,1024);//TODO
    while (ts_rb_pop(lth->ringbuffer,&message))
    {
        size_t log_len = message.header.msg_len - sizeof(ts_rb_header_t);
        const char *msg = message.payload.payload_bytes;
        
        const char *sev_str = "UNKN";
        switch (message.header.severity) {
            case LOG_DEBUG: sev_str = "DEBUG"; break;
            case LOG_INFO:  sev_str = "INFO "; break;
            case LOG_WARN:  sev_str = "WARN "; break;
            case LOG_ERROR: sev_str = "ERROR"; break;
            case LOG_FATAL: sev_str = "FATAL"; break;
            case LOG_NONE:  sev_str = "     "; break; 
        }
        
        int written = fprintf(lth->file, "[%s] %.*s\n", sev_str, (int)log_len, msg);
        if (written > 0)
        {
            lth->written_bytes += written;
        }

        if (message.header.severity >= LOG_ERROR) 
        {
            fflush(lth->file); // Prima svuota la libc
            fdatasync(fileno(lth->file)); // Poi forza il disco
        }

        if (lth->written_bytes < F_MAX_SIZE)
        {
            continue;
        }

        if (lth->compress_q == NULL)
        {
            // Se non c'è il compressore, azzeriamo solo i byte? 
            // ATTENZIONE: Questo lascerebbe crescere il file all'infinito!
            // Di solito, se non c'è compressione, si svuota il file o si ruota e basta.
            continue; 
        }

        fflush(lth->file);
        fclose(lth->file);
        
        char rotating_file[P_SIZE] = {0};
        create_rotate_file_name(lth->path, rotating_file, P_SIZE);
        
        if (rename(lth->path, rotating_file) != 0)
        {
            fprintf(stderr, "Error: could not rename %s to %s.\n", lth->path, rotating_file);
            // Non fare return NULL! Se fallisce, riapri almeno il vecchio e prova a continuare
            lth->file = fopen(lth->path, "a");
            lth->written_bytes = 0;
            continue;
        }

        char *file_to_compress = strdup(rotating_file);
        if (file_to_compress) {
            ts_queue_push(lth->compress_q, LOG_NONE, file_to_compress);
            
        }

        lth->file = fopen(lth->path, "a");
        if (!lth->file)
        {
            fprintf(stderr, "Fatal Error: Logger thread cannot reopen %s\n", lth->path);
            return NULL;
        }
        
        lth->written_bytes = 0;
        free(file_to_compress);
    }
    
    atomic_store(&lth->running, false);
    fclose(lth->file);
    lth->file = NULL;
    fprintf(stdout,"Logger end.\n");
    return NULL;
}
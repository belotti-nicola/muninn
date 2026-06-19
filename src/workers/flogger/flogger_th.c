#include <string.h>
#include <unistd.h>

#include <muninn.h>

#include <internal/ts_queue.h>
#include <internal/flogger_th.h>
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


void *flogger_loop_fn(void *arg)
{
    if(arg == NULL) return NULL;

    flogger_th_data *lth = (flogger_th_data *)arg;

    if(lth->muninn == NULL || lth->path == NULL) return NULL;

    lth->file = fopen(lth->path, "a");
    if (!lth->file)
    {
        fprintf(stderr,"Error: could not open file '%s'",lth->path);
        return NULL;
    }

    ts_queue_t      *q = &lth->muninn->flogger_q;
    queue_message_t *m =  lth->muninn->flogger_m;

    char qm[1024] = {0};//TODO
    char qm_compressor[P_SIZE] = {0};

    int i=0;
    while (ts_queue_pop(q,m))
    {      
        int written = fprintf(lth->file, "[INSERT] %s\n", m->data);
        if (written > 0)
        {
            lth->written_bytes += written;
        }

        //todo
        //if (message.header.severity >= LOG_ERROR) 
        if(i%10 == 0)
        {
            fflush(lth->file); // Prima svuota la libc
            fdatasync(fileno(lth->file)); // Poi forza il disco
        }
        i++;
        if (lth->written_bytes < F_MAX_SIZE)
        {
            continue;
        }

        if (&lth->muninn->compressor_q == NULL)
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
            ts_queue_push(&lth->muninn->compressor_q, LOG_NONE, file_to_compress);
            
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
    
    fclose(lth->file);
    lth->file = NULL;
    fprintf(stdout,"Logger end.\n");
    return NULL;
}

void *flogger_stop_fn(void *arg)
{
    if(arg == NULL) return NULL;

    flogger_th_data *lth_data = (flogger_th_data *)arg;

    ts_queue_stop(&lth_data->muninn->flogger_q);
    return NULL;
}

//TODO
void *flogger_join_fn(void *arg)
{  
    if(arg == NULL) return NULL;
   
    return NULL;
}

void *flogger_post_fn(void *context, void *data)
{
    if(context == NULL || data == NULL) return NULL;

    flogger_th_data *lth_data = (flogger_th_data *)context;
    char *message_content = (char *)data;

    ts_rb_message_t message = {0};
    size_t full_len = strlen(message_content) + sizeof(ts_rb_header_t);
    uint64_t ts = timestamp_u64();
    uint8_t sev = (uint8_t)1;//TODO
    ts_rb_message_set(&message,full_len,ts,sev,(uint8_t *)message_content);

    ts_queue_push(&lth_data->muninn->flogger_q,1,data);

    return NULL;
}
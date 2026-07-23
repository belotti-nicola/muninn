#include <string.h>
#include <unistd.h>

#include <muninn.h>

#include <internal/ts_rb_decoder.h>

#include <internal/ts_queue.h>
#include <internal/flogger_th.h>
#include <internal/timestamp_gen.h>
#include <stdlib.h>

#define TS_SIZE 16
#define ROTATING_SUFFIX ".rotating"
#define BUFFSIZE 65536 * 2

void create_rotate_file_name(const char *in, char *out, size_t out_size)
{
    char ts[TS_SIZE];
    snprintf(ts, sizeof(ts), "%ld", (long)timestamp_now_ms());

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
    if(lth->path == NULL) return NULL;

    lth->file = fopen(lth->path, "a");
    if (!lth->file) {
        fprintf(stderr, "Error: could not open file '%s'\n", lth->path);
        return NULL;
    }

    // 2. DISATTIVIAMO il buffer interno della libc. Scrittura diretta a bassa latenza!
    setvbuf(lth->file, NULL, _IONBF, 0);

    if(lth->reading_queue == NULL) {
        fprintf(stderr, "Error: reading Queue is null\n");
        fclose(lth->file);
        return NULL;
    }
    
    ts_queue_t *q   = lth->reading_queue;
    ts_queue_t *out = lth->output_queue;

    char data[LOG_MESSAGE_SIZE];
    queue_message_t msg = {0};
    setup_queue_message(&msg, data, LOG_MESSAGE_SIZE);

    // Allocato una volta sola all'avvio del thread
    char buffer[BUFFSIZE]; 
    size_t buffer_size = 0;


    size_t max_dim      = BUFFSIZE;
    size_t popped_bytes = 0;
    while (ts_queue_n_pop(q, buffer, max_dim, &popped_bytes))
    {
        if (popped_bytes == 0) continue;

        //write
        size_t written = fwrite(buffer, 1, popped_bytes , lth->file);
        lth->written_bytes += written;

        //rotation
        if (lth->written_bytes >= F_MAX_SIZE)
        {
            // Svuotiamo i buffer interni di I/O della libc prima della chiusura
            fflush(lth->file);
            fclose(lth->file);
            
            char rotating_file[P_SIZE] = {0};
            create_rotate_file_name(lth->path, rotating_file, P_SIZE);
            
            if (rename(lth->path, rotating_file) != 0)
            {
                fprintf(stderr, "Error: could not rename %s.\n", lth->path);
                lth->file = fopen(lth->path, "a");
                if (!lth->file) {
                    fprintf(stderr, "Fatal Error: Cannot reopen %s after failed rename\n", lth->path);
                    return NULL;
                }
                setvbuf(lth->file, NULL, _IONBF, 0);
                lth->written_bytes = 0;
                continue;
            }

            // Notifica il file ruotato alla coda di output (es. per flogger/flipper/compressore)
            ts_queue_push(out, LOG_NONE, rotating_file);
            
            // Riapertura del file di log principale
            lth->file = fopen(lth->path, "a");
            if (!lth->file) {
                fprintf(stderr, "Fatal Error: Cannot reopen %s\n", lth->path);
                return NULL;
            }

            // Riapplichiamo il no-buffering sulla nuova stream appena aperta
            setvbuf(lth->file, NULL, _IONBF, 0);
            lth->written_bytes = 0;
        }
    }

    if (lth->file != NULL)
    {
        if (lth->written_bytes >= F_MAX_SIZE)
        {
            fflush(lth->file);
            fclose(lth->file);

            char rotating_file[P_SIZE] = {0};
            create_rotate_file_name(lth->path, rotating_file, P_SIZE);
            
            if (rename(lth->path, rotating_file) == 0)
            {
                ts_queue_push(out, LOG_NONE, rotating_file);//TODO
            }
            else
            {
                fprintf(stderr, "Error: final rename failed for %s.\n", lth->path);
            }
        }
        else
        {
            fflush(lth->file);
            fclose(lth->file);
        }

        lth->file = NULL;
    }

    fprintf(stdout, "Logger end.\n");
    return NULL;
}

void *flogger_stop_fn(void *arg)
{
    if(arg == NULL) return NULL;

    flogger_th_data *lth_data = (flogger_th_data *)arg;

    ts_queue_stop(lth_data->reading_queue);
    return NULL;
}

void *flogger_post_fn(void *context, void *data,size_t data_size)
{
    if(context == NULL || data == NULL) return NULL;

    flogger_th_data *lth_data = (flogger_th_data *)context;
    char *message_content = (char *)data;

    ts_queue_n_push(lth_data->reading_queue, 1, (const uint8_t *)message_content, data_size);

    return NULL;
}
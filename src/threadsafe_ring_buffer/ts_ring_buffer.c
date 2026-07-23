#include <internal/ts_ring_buffer.h>
#include <internal/ts_rb_encoder.h>
#include <internal/ts_rb_decoder.h>
#include <internal/timestamp_gen.h>
#include <internal/ts_rb_message.h>
#include <string.h>
#include <internal/protocols/messages/encoded_data_mask.h>
#include <internal/protocols/messages/codec_utility.h>

#include <internal/muninn_int.h>

void ts_rb_setup(ts_ring_buffer_t *tsrb,uint8_t *buffer, size_t buffer_dim)
{
    if(tsrb == NULL || buffer == NULL) return;

    rb_setup(&tsrb->ring_buffer,buffer,buffer_dim);
    tsrb->stop = false;

    pthread_mutex_init(&tsrb->mutex, NULL);
    pthread_cond_init(&tsrb->empty, NULL);
    pthread_cond_init(&tsrb->full, NULL);
}

bool ts_rb_pop(ts_ring_buffer_t *tsrb, uint8_t *out, size_t out_size, size_t *bytes_popped)
{
    if (tsrb == NULL || out == NULL || bytes_popped == NULL) return false;

    pthread_mutex_lock(&tsrb->mutex);

    while (tsrb->ring_buffer.current_size < sizeof(uint16_t) && !tsrb->stop) {
        pthread_cond_wait(&tsrb->empty, &tsrb->mutex);
    }

    if (tsrb->stop && tsrb->ring_buffer.current_size < sizeof(uint16_t)) {
        pthread_mutex_unlock(&tsrb->mutex);
        return false;
    }

    uint8_t headers[sizeof(uint16_t)];
    if (!rb_pop(&tsrb->ring_buffer, headers, sizeof(headers))) {
        pthread_mutex_unlock(&tsrb->mutex);
        return false;
    }

    mm_decoder_t decoder;
    decoder_setup(&decoder, headers, sizeof(headers));

    uint16_t message_len = 0;
    decode_u16(&decoder, &message_len);

    size_t bytes_to_read = message_len;
    bool is_truncated = false;

    if (bytes_to_read > out_size) {
        bytes_to_read = out_size; // Tranchiamo per non sfondare lo stack!
        is_truncated = true;
    }

    // Leggiamo solo quello che il buffer di destinazione può contenere
    if (!rb_pop(&tsrb->ring_buffer, out, bytes_to_read)) {
        pthread_mutex_unlock(&tsrb->mutex);
        return false;
    }

    if (is_truncated) {
        size_t excess_bytes = message_len - out_size;
        uint8_t trash[128];
        while (excess_bytes > 0) {
            size_t chunk = (excess_bytes > sizeof(trash)) ? sizeof(trash) : excess_bytes;
            rb_pop(&tsrb->ring_buffer, trash, chunk);
            excess_bytes -= chunk;
        }
    }

    *bytes_popped = bytes_to_read;

    pthread_cond_signal(&tsrb->full);
    pthread_mutex_unlock(&tsrb->mutex);
    return true;
}

bool ts_rb_push(ts_ring_buffer_t* tsrb, const uint8_t *buffer, size_t buffer_size)
{
    if (tsrb == NULL || buffer == NULL || buffer_size == 0) 
    {
        return false;
    }

    size_t header_size = sizeof(uint16_t);
    size_t total_msg_size = header_size + buffer_size;

    // Preariamo l'header fuori dalla sezione critica (per ridurre il tempo di lock)
    uint8_t header[sizeof(uint16_t)] = {0}; 
    mm_encoder_t encoder = {0};
    encoder_setup(&encoder, header, header_size);

    if (encode_u16(&encoder, (uint16_t)buffer_size) == false)
    {
        return false;
    }
    
    pthread_mutex_lock(&tsrb->mutex);
 
    // 1. Aspettiamo che ci sia spazio libero per l'INTERO pacchetto (Header + Payload)
    // Assicurati che rb_free_space (o la tua funzione equivalente) restituisca lo spazio libero rimanente!
    while (rb_peek(&tsrb->ring_buffer) < total_msg_size && !tsrb->stop) 
    {
        pthread_cond_wait(&tsrb->full, &tsrb->mutex);
    }

    // Se il sistema è in fase di arresto, rilasciamo e usciamo
    if (tsrb->stop)
    {
        pthread_mutex_unlock(&tsrb->mutex);
        return false;
    }
        
    // 2. Lo spazio è ORA GARANTITO: Scriviamo atomicamente sia l'header che il payload
    rb_push(&tsrb->ring_buffer, encoder.buffer, encoder.current_size);
    rb_push(&tsrb->ring_buffer, buffer, buffer_size);

    // 3. Notifichiamo il Gateway che c'è almeno un messaggio pronto
    pthread_cond_signal(&tsrb->empty);

    pthread_mutex_unlock(&tsrb->mutex);

    return true;
}

void ts_rb_stop(ts_ring_buffer_t *tsrb)
{
    if(tsrb == NULL) return;

    pthread_mutex_lock(&tsrb->mutex);

    tsrb->stop = true;
    pthread_cond_broadcast(&tsrb->empty);
    pthread_cond_broadcast(&tsrb->full);
    
    pthread_mutex_unlock(&tsrb->mutex);
}

void ts_rb_release(ts_ring_buffer_t *tsrb)
{
    if(tsrb == NULL) return;
    
    pthread_mutex_destroy(
        &(tsrb->mutex)
    );
     pthread_cond_destroy(
        &(tsrb->empty)
    );
}
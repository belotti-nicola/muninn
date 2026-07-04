#include <internal/ts_ring_buffer.h>
#include <internal/ts_rb_encoder.h>
#include <internal/ts_rb_decoder.h>
#include <internal/timestamp_gen.h>
#include <internal/ts_rb_message.h>
#include <string.h>

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

bool ts_rb_pop(ts_ring_buffer_t *tsrb, ts_rb_message_t *out)
{
    if(tsrb == NULL || out == NULL) return false;

    pthread_mutex_lock(&tsrb->mutex);

    while (tsrb->ring_buffer.current_size == 0 && !tsrb->stop) {
        pthread_cond_wait(&tsrb->empty, &tsrb->mutex);
    }

    if (tsrb->stop && tsrb->ring_buffer.current_size == 0) {
        pthread_mutex_unlock(&tsrb->mutex);
        return false;
    }
    
    uint8_t headers[sizeof(ts_rb_header_t)];
    if (rb_pop(&tsrb->ring_buffer, headers, sizeof(ts_rb_header_t)) == 0) {
        pthread_mutex_unlock(&tsrb->mutex);
        return false;
    }

    decoder_t decoder = {0};
    decoder_setup(&decoder, headers, sizeof(ts_rb_header_t));
    
    uint32_t tmp_msg_len = 0;
    uint64_t tmp_ts = 0;
    uint8_t  tmp_severity = 0;

    decode_u32(&decoder, &tmp_msg_len);
    decode_u64(&decoder, &tmp_ts);
    decode_u8(&decoder, &tmp_severity);

    size_t payload_len = tmp_msg_len - sizeof(ts_rb_header_t);
    
    size_t max_buffer_capacity = LOG_RB_SIZE - 1; 
    size_t bytes_to_copy = (payload_len > max_buffer_capacity) ? max_buffer_capacity : payload_len;

    if (bytes_to_copy > 0) {
        rb_pop(&tsrb->ring_buffer, out->payload.payload_bytes, bytes_to_copy);
        out->payload.payload_bytes[bytes_to_copy] = '\0'; 
    }

    size_t leftover_bytes = payload_len - bytes_to_copy;
    if (leftover_bytes > 0) {
        uint8_t trash_bin[128]; 
        while (leftover_bytes > 0) {
            size_t chunk = (leftover_bytes > sizeof(trash_bin)) ? sizeof(trash_bin) : leftover_bytes;
            rb_pop(&tsrb->ring_buffer, trash_bin, chunk);
            leftover_bytes -= chunk;
        }
    }

    out->header.msg_len  = tmp_msg_len;
    out->header.severity = tmp_severity;
    out->header.ts       = tmp_ts;

    pthread_cond_signal(&tsrb->full);

    pthread_mutex_unlock(&tsrb->mutex);
    return true;
}

bool ts_rb_push(ts_ring_buffer_t* tsrb, const ts_rb_message_t *message)
{
    if(tsrb == NULL                                 || 
       message == NULL                              ||
       message->payload.payload_bytes == NULL       ||
       message->header.msg_len == 0)
    {
        return false;
    }


    uint32_t full_len = message->header.msg_len;
    uint64_t ts = message->header.ts;
    uint8_t  sev = message->header.severity;
    size_t   payload_length = full_len - sizeof(ts_rb_header_t);

    uint8_t header_encoded[sizeof(ts_rb_header_t)] = {0}; // ENCODING ONLY HEADERS
    memset(header_encoded,0,sizeof(ts_rb_header_t));      // SINCE DATA IS MEMCPIED AS-IS 
    
    encoder_t encoder = {0}; 
    encoder_setup(&encoder,header_encoded,sizeof(ts_rb_header_t));
    encode_u32(&encoder,full_len);
    encode_u64(&encoder,ts);
    encode_u8(&encoder,sev);
    
    pthread_mutex_lock(&tsrb->mutex);
 
    while (rb_peek(&tsrb->ring_buffer) < message->header.msg_len && !tsrb->stop) 
    {
        pthread_cond_wait(&tsrb->full, &tsrb->mutex);
    }

    if (tsrb->stop)
    {
        pthread_mutex_unlock(&tsrb->mutex);
        return false;
    }
        
    bool push_rc;
    push_rc = rb_push(&tsrb->ring_buffer,encoder.buffer,encoder.current_size);
    if(push_rc == false)
    {
        pthread_mutex_unlock(&tsrb->mutex);
        return false;
    }
    push_rc = rb_push(&tsrb->ring_buffer,message->payload.payload_bytes,full_len - sizeof(ts_rb_header_t));
    if(push_rc == false)
    {
        pthread_mutex_unlock(&tsrb->mutex);
        return false;//TODO REVERT
    }

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
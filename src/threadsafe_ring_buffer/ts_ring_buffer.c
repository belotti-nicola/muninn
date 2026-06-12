#include <internal/ts_ring_buffer.h>
#include <internal/ts_rb_encoder.h>
#include <internal/ts_rb_decoder.h>
#include <internal/timestamp_gen.h>
#include <internal/ts_rb_message.h>
#include <string.h>



void ts_rb_setup(ts_ring_buffer_t *tsrb,uint8_t *buffer, size_t buffer_dim)
{
    rb_setup(&tsrb->ring_buffer,buffer,buffer_dim);
    tsrb->stop = false;

    pthread_mutex_init(&tsrb->mutex, NULL);
    pthread_cond_init(&tsrb->empty, NULL);
}

bool ts_rb_pop(ts_ring_buffer_t *tsrb,ts_rb_message_t *out)
{
    pthread_mutex_lock(&tsrb->mutex);

    while (tsrb->ring_buffer.current_size == 0 && !tsrb->stop)
    {
        pthread_cond_wait(&tsrb->empty, &tsrb->mutex);
    }

    if (tsrb->stop && tsrb->ring_buffer.current_size == 0)
    {
        pthread_mutex_unlock(&tsrb->mutex);
        return false;
    }

    size_t max_dim = sizeof(out->header) + out->payload.payload_bytes_max_size;
    uint8_t buffer[max_dim];memset(buffer, 0, max_dim);
    decoder_t decoder = {0};
    decoder_setup(&decoder,buffer,max_dim);

    uint32_t tmp_msg_len = 0;
    uint64_t tmp_ts = 0;
    uint8_t  tmp_severity = 0;

    rb_pop(&tsrb->ring_buffer,buffer,4);
    decode_u32(&decoder,&tmp_msg_len);
    rb_pop(&tsrb->ring_buffer,buffer+4,tmp_msg_len);
    decode_u64(&decoder,&tmp_ts);
    decode_u8(&decoder,&tmp_severity);

    size_t last_bytes = (size_t)tmp_msg_len - sizeof(ts_rb_header_t);
    if(out->payload.payload_bytes_max_size < last_bytes) 
    { 
        last_bytes = out->payload.payload_bytes_max_size;
    }
    decode_bytes(
        &decoder,
        out->payload.payload_bytes,
        last_bytes
    );

    out->header.msg_len  = tmp_msg_len;
    out->header.severity = tmp_severity;
    out->header.ts       = tmp_ts;

    pthread_mutex_unlock(&tsrb->mutex);
    return true;
}

bool ts_rb_push(ts_ring_buffer_t* tsrb, const ts_rb_message_t *message)
{
    if(tsrb == NULL                                 || 
       message->payload.payload_bytes == NULL       ||
       message->payload.payload_bytes_max_size == 0 ||
       message->header.msg_len == 0)
    { 
        return false;
    }
    
    pthread_mutex_lock(&tsrb->mutex);

    if (tsrb->stop)
    {
        pthread_mutex_unlock(&tsrb->mutex);
        return false;
    }

    size_t available_bytes = rb_peek(&tsrb->ring_buffer);
    if( available_bytes <= sizeof(ts_rb_header_t) )
    {
        pthread_mutex_unlock(&tsrb->mutex);
        return false;
    }

    uint32_t full_len = message->header.msg_len;
    uint64_t ts = timestamp_u64();
    uint8_t  sev = message->header.severity;
    size_t   payload_length = full_len - sizeof(ts_rb_header_t);

    uint8_t encoder_buff[full_len];
    memset(encoder_buff,0,full_len);
    
    encoder_t encoder = {0}; 
    encoder_setup(&encoder,encoder_buff,full_len);
    encode_u32(&encoder,full_len);
    encode_u64(&encoder,ts);
    encode_u8(&encoder,sev);
    encode_bytes(&encoder,message->payload.payload_bytes,payload_length);

    size_t pushed_bytes = rb_push(&tsrb->ring_buffer,encoder.buffer,encoder.current_size);
    if(pushed_bytes > 0)
    {
        pthread_cond_signal(&tsrb->empty);
    }

    pthread_mutex_unlock(&tsrb->mutex);
    return true;

}
void ts_rb_stop(ts_ring_buffer_t *tsrb)
{
    pthread_mutex_lock(&tsrb->mutex);

    tsrb->stop = true;
    pthread_cond_broadcast(&tsrb->empty);
    
    pthread_mutex_unlock(&tsrb->mutex);
}
void ts_rb_release(ts_ring_buffer_t *tsrb)
{
    pthread_mutex_destroy(
        &(tsrb->mutex)
    );
     pthread_cond_destroy(
        &(tsrb->empty)
    );
}
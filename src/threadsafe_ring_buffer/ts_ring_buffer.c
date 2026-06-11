#include <internal/ts_ring_buffer.h>
#include <internal/ts_rb_encoder.h>
#include <internal/ts_rb_decoder.h>
#include <internal/timestamp_gen.h>



void ts_rb_setup(ts_ring_buffer_t *tsrb,uint8_t *buffer, size_t buffer_dim)
{
    rb_setup(&tsrb->ring_buffer,buffer,buffer_dim);
    tsrb->stop = false;

    pthread_mutex_init(&tsrb->mutex, NULL);
    pthread_cond_init(&tsrb->empty, NULL);
}

bool ts_rb_pop(ts_ring_buffer_t *tsrb, uint8_t *out, size_t out_dim)
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

    uint8_t  severity;
    uint32_t len;
    uint64_t timestamp;

    uint8_t buffer[1024];//TODO
    decoder_t decoder = {0};
    decoder_setup(&decoder,buffer,1024);//TODO

    rb_pop(&tsrb->ring_buffer,buffer,4);
    decode_u32(&decoder,&len);
    rb_pop(&tsrb->ring_buffer,buffer,len);
    decode_u64(&decoder,&timestamp);
    decode_u8(&decoder,&severity);
    decode_bytes(&decoder,out,len - 13);//todo

    pthread_mutex_unlock(&tsrb->mutex);
    return true;
}
bool ts_rb_push(ts_ring_buffer_t *tsrb, uint8_t *buffer, size_t buffer_dim)
{
    if(tsrb == NULL || buffer == NULL || buffer_dim == 0)
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
    if( available_bytes <= 13 )//todo
    {
        pthread_mutex_unlock(&tsrb->mutex);
        return false;
    }

    uint32_t full_len = 13 + buffer_dim;
    uint64_t ts = timestamp_u64();
    uint8_t  sev = 127;//TODO

    uint8_t encoder_buff[1024];//TODO
    encoder_t encoder = {0}; 
    encoder_setup(&encoder,encoder_buff,1024);
    encode_u32(&encoder,full_len);
    encode_u64(&encoder,ts);
    encode_u8(&encoder,sev);
    encode_bytes(&encoder,buffer,buffer_dim);

    size_t pushed_bytes = rb_push(&tsrb->ring_buffer,encoder.buffer,encoder.current_size);
    if(pushed_bytes > 0)
    {
        pthread_cond_signal(&tsrb->empty);
    }

    pthread_mutex_unlock(&tsrb->mutex);
    return true;//todo

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
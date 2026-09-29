#include <internal/ts_ring_buffer.h>

#include <internal/protocols/muninn_messages/muninn_message_header.h>
#include <internal/protocols/muninn_messages/muninn_message_payload.h>
#include <internal/protocols/muninn_messages/muninn_header_codec.h>
#include <internal/protocols/muninn_messages/muninn_payload_codec.h>
#include <internal/protocols/muninn_messages/muninn_message.h>
#include <internal/protocols/muninn_messages/muninn_codec.h>



void ts_rb_setup(ts_ring_buffer_t *tsrb, uint8_t *buffer, size_t buffer_dim)
{
    if (tsrb == NULL || buffer == NULL) return;

    rb_setup(&tsrb->ring_buffer, buffer, buffer_dim);
    tsrb->stop = false;

    pthread_mutex_init(&tsrb->mutex, NULL);
    pthread_cond_init(&tsrb->empty, NULL);
    pthread_cond_init(&tsrb->full, NULL);
}

bool ts_rb_push(ts_ring_buffer_t* tsrb, const uint8_t *buffer, size_t buffer_size)
{
    if (tsrb == NULL || buffer == NULL || buffer_size == 0) return false;

    pthread_mutex_lock(&tsrb->mutex);
    
    while (rb_available_space(&tsrb->ring_buffer) < buffer_size && !tsrb->stop) 
    {
        pthread_cond_wait(&tsrb->full, &tsrb->mutex);
    }
        
    if (tsrb->stop || rb_available_space(&tsrb->ring_buffer) < buffer_size)
    {
        pthread_mutex_unlock(&tsrb->mutex);
        return false;
    }

    if (rb_push(&tsrb->ring_buffer, buffer, buffer_size) == false)
    {
        pthread_mutex_unlock(&tsrb->mutex);
        return false;
    }
    
    pthread_cond_broadcast(&tsrb->empty);

    pthread_mutex_unlock(&tsrb->mutex);
    return true;
}

bool ts_rb_pop(ts_ring_buffer_t *tsrb, uint8_t *out, size_t out_size, size_t *bytes_to_pop)
{
    if (tsrb == NULL || out == NULL || bytes_to_pop == 0) return false;

    pthread_mutex_lock(&tsrb->mutex);

    size_t header_size = 2;
    uint8_t header[2] = {0};

    while (tsrb->ring_buffer.current_size < sizeof(uint16_t) && !tsrb->stop) 
    {
        pthread_cond_wait(&tsrb->empty, &tsrb->mutex);
    }

    if (tsrb->stop && tsrb->ring_buffer.current_size < sizeof(uint16_t)) 
    {
        pthread_mutex_unlock(&tsrb->mutex);
        return false;
    }

    if(rb_peek(&tsrb->ring_buffer,header,2) == false)
    {
        pthread_mutex_unlock(&tsrb->mutex);
        return false;
    }
    if(rb_advance(&tsrb->ring_buffer,2) == false)
    {
        pthread_mutex_unlock(&tsrb->mutex);
        return false;
    }

    muninn_header muninn_header = {0};
    if(muninn_header_decode(header,2,&muninn_header) == false)
    {
        pthread_cond_broadcast(&tsrb->full);
        pthread_mutex_unlock(&tsrb->mutex);
        return false;
    }

    uint8_t payload_buffer[2048] = {0};
    size_t payload_len = muninn_header.payload_len;
    if(payload_len == 0 || payload_len > out_size)
    {
        pthread_cond_broadcast(&tsrb->full);
        pthread_mutex_unlock(&tsrb->mutex);
        return false;
    }
    if(rb_peek(&tsrb->ring_buffer,payload_buffer,payload_len) == false)
    {
        pthread_mutex_unlock(&tsrb->mutex);
        pthread_cond_broadcast(&tsrb->full);
        return false;
    }
    if(rb_advance(&tsrb->ring_buffer,payload_len) == false)
    {
        pthread_cond_broadcast(&tsrb->full);
        pthread_mutex_unlock(&tsrb->mutex);
        return false;
    }

    muninn_payload muninn_payload = {0};
    uint8_t msg[2048] = {0}; //todo
    muninn_payload.msg = msg;
    if(muninn_payload_decode(payload_buffer,2048,&muninn_payload) == false)
    {
        pthread_cond_broadcast(&tsrb->full);
        pthread_mutex_unlock(&tsrb->mutex);
        return false;
    }

    memcpy(out,muninn_payload.msg,muninn_payload.msg_len);
    
    *bytes_to_pop = muninn_payload.msg_len;

    pthread_cond_broadcast(&tsrb->full);

    pthread_mutex_unlock(&tsrb->mutex);
    return true;
}

void ts_rb_stop(ts_ring_buffer_t *tsrb)
{
    if (tsrb == NULL) return;

    pthread_mutex_lock(&tsrb->mutex);

    tsrb->stop = true;
    pthread_cond_broadcast(&tsrb->empty);
    pthread_cond_broadcast(&tsrb->full);
    
    pthread_mutex_unlock(&tsrb->mutex);
}

void ts_rb_release(ts_ring_buffer_t *tsrb)
{
    if (tsrb == NULL) return;
    
    pthread_mutex_destroy(&(tsrb->mutex));
    pthread_cond_destroy(&(tsrb->empty));
    pthread_cond_destroy(&(tsrb->full));
}

bool ts_rb_peek(ts_ring_buffer_t *tsrb, uint8_t *out, size_t peekable_bytes)
{
    if ( tsrb == NULL || out == NULL || peekable_bytes == 0) return false;

    pthread_mutex_lock(&tsrb->mutex);

    while ( !tsrb->stop  && tsrb->ring_buffer.current_size < peekable_bytes && !tsrb->stop) 
    {
        pthread_cond_wait(&tsrb->empty, &tsrb->mutex);
    }

    if ( tsrb->stop && tsrb->ring_buffer.current_size == 0 ) 
    {
        pthread_mutex_unlock(&tsrb->mutex);
        return false;
    }

    if ( rb_peek( &tsrb->ring_buffer, out, peekable_bytes) == false )
    {
        pthread_mutex_unlock(&tsrb->mutex);

        return false;
    }
    
    pthread_mutex_unlock(&tsrb->mutex);

    return true;
}

bool ts_rb_advance(ts_ring_buffer_t *tsrb, size_t bytes_advanced)
{
    if ( tsrb == NULL || bytes_advanced == 0) return false;

    pthread_mutex_lock(&tsrb->mutex);

    while ( !tsrb->stop && tsrb->ring_buffer.current_size < bytes_advanced && !tsrb->stop) 
    {
        pthread_mutex_unlock(&tsrb->mutex);
        
        return false;
    }

    if ( tsrb->stop && tsrb->ring_buffer.current_size == 0 ) 
    {
        pthread_mutex_unlock(&tsrb->mutex);
        
        return false;
    }

    if ( rb_advance( &tsrb->ring_buffer, bytes_advanced ) == false )
    {
        pthread_mutex_unlock(&tsrb->mutex);

        return false;
    }

    pthread_cond_broadcast(&tsrb->full);

    pthread_mutex_unlock(&tsrb->mutex);
    
    return true;

}
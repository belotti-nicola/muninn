#include <internal/ts_ring_buffer.h>

#include <internal/ts_rb_message.h>
#include <internal/muninn_int.h>
#include <string.h>


//TODO SIMPLIFY ALL INCLUDE
#include <internal/protocols/muninn_messages/muninn_message_header.h>
#include <internal/protocols/muninn_messages/muninn_message_payload.h>
#include <internal/protocols/muninn_messages/muninn_header_codec.h>
#include <internal/protocols/muninn_messages/muninn_payload_codec.h>
#include <internal/protocols/muninn_messages/muninn_codec.h>
#include <internal/protocols/muninn_messages/muninn_message.h>
#include <internal/protocols/muninn_messages/muninn_message_mask.h>



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

    uint8_t decoder_buffer[2048] = {0};//todo

    size_t header_size = sizeof(muninn_header);
    if(rb_pop(&tsrb->ring_buffer,decoder_buffer,header_size) == false)
    {
        return false;
    }
    muninn_header header = {0};
    muninn_header_decode(decoder_buffer,header_size,&header);

    size_t payload_size = (size_t)header.payload_len;//todo
    if(rb_pop(&tsrb->ring_buffer,decoder_buffer,payload_size) == false)
    {
        return false;
    }
    muninn_payload payload = {0};
    uint8_t msg_buffer[2048];
    payload.msg = msg_buffer;
    muninn_payload_decode(decoder_buffer,payload_size,&payload);

    memcpy(out,payload.msg,payload.msg_len);
    *bytes_popped = payload.msg_len;

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

    pthread_mutex_lock(&tsrb->mutex);
    
    //todo
    uint8_t encoded[2048] = {0};
    size_t encoded_size = 2048;
    muninn_message message = {0};
    muninn_header header = {0};
    header.payload_len = buffer_size+6;
    muninn_payload payload = {0};
    payload.msg = buffer;
    payload.msg_len = buffer_size;
    
    muninn_message_set(&message,&header,&payload);
    
    if(muninn_messages_encode(&message,MEDM_MESSAGE,encoded,&encoded_size) == false)
    {
        pthread_mutex_unlock(&tsrb->mutex);

        return false;
    }

    while (rb_available_space(&tsrb->ring_buffer) < encoded_size && !tsrb->stop) 
    {
        pthread_cond_wait(&tsrb->full, &tsrb->mutex);
    }
        
    if(rb_push(&tsrb->ring_buffer,encoded,encoded_size) == false)
    {
        pthread_mutex_unlock(&tsrb->mutex);

        return false;
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
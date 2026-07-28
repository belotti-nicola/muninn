#include <internal/ring_buffer.h>
#include <string.h>

bool rb_setup(ring_buffer *rb, uint8_t *data, size_t data_size)
{
    if (rb == NULL || data == NULL) return false;

    if (data_size == 0) return false;

    rb->data         = data;
    rb->data_size    = data_size;
    rb->start        = 0;
    rb->end          = 0;
    rb->current_size = 0;

    return true;
}

size_t rb_peek(ring_buffer *rb)
{
    if(rb == NULL)
    {
        return 0;
    }

    return rb->data_size - rb->current_size;
}

size_t rb_push(ring_buffer *rb, const uint8_t *buff, size_t buff_size)
{
    if (rb == NULL || buff == NULL || buff_size == 0) return 0;

    size_t available_space = rb_peek(rb);
    if (available_space < buff_size)
    {
        return 0; // Drop policy
    }

    size_t offset = rb->end;
    size_t bytes_to_copy = buff_size;

    if (offset + bytes_to_copy > rb->data_size)
    {
        size_t bytes_to_right = rb->data_size - offset;      
        memcpy(rb->data + offset, buff, bytes_to_right);
        buff          += bytes_to_right;
        bytes_to_copy -= bytes_to_right;
        offset         = 0; 
    }
    
    memcpy(rb->data + offset, buff, bytes_to_copy);
    rb->current_size += buff_size;
    rb->end = (offset + bytes_to_copy) % rb->data_size;
    return buff_size; 
}

size_t rb_pop(ring_buffer *rb, uint8_t *out, size_t out_size)
{
    if (rb == NULL || out == NULL || out_size == 0 ) 
    {
        return 0;
    }
    
    if( out_size > rb->current_size)
    {
        return 0; //for sake of simplicity
    }

    uint8_t *ptr = rb->data;
    size_t offset = rb->start;
    size_t bytes_to_read = out_size;

    if (offset + bytes_to_read > rb->data_size)
    {
        size_t available_bytes_to_right = rb->data_size - offset;
        memcpy(out, ptr + offset, available_bytes_to_right);
        out           += available_bytes_to_right;
        bytes_to_read -= available_bytes_to_right;
        offset         = 0;
    }
    
    memcpy(out, ptr + offset, bytes_to_read);
    rb->current_size -= out_size;
    rb->start = (offset + bytes_to_read) % rb->data_size;
    return out_size; 
}

size_t rb_available_space(const ring_buffer *rb)
{
    if (rb == NULL) return 0;
    return rb->data_size - rb->current_size;
}
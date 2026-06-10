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

size_t rb_available_data(ring_buffer *rb)
{
    if(rb == NULL)
    {
        return 0;
    }

    return rb->data_size - rb->current_size;
}

size_t rb_push(ring_buffer *rb, const uint8_t *buff, size_t buff_size)
{
    if (rb == NULL || buff == NULL || buff_size == 0) return false;
    const size_t MAX_INDEX = rb->data_size - 1;

    size_t available_data = rb_available_data(rb);
    if(available_data < buff_size)
    {
        //drop policy!
        return 0;
    }

    size_t offset = rb->start;
    size_t bytes_to_copy = buff_size;
    if(offset + bytes_to_copy > MAX_INDEX - 1)
    {
        size_t tmp = MAX_INDEX - offset;
        memcpy(rb->data + offset,buff,tmp);
        buff          += tmp;
        bytes_to_copy -= tmp;
        offset         = 0;
        rb->end        = 0;
    }
    memcpy(rb->data + offset,buff,bytes_to_copy);

    rb->current_size += buff_size;
    rb->end          += offset + bytes_to_copy;

    return bytes_to_copy;
}

size_t rb_pop(ring_buffer *rb, uint8_t *out, size_t out_size)
{
    if (rb == NULL || out == NULL || out_size == 0 || out_size > rb->data_size || rb->current_size == 0) return 0;
    const size_t MAX_INDEX = rb->data_size - 1;

    uint8_t *ptr  = rb->data;
    size_t offset = rb->start;
    size_t bytes_to_read = out_size;
    if(offset + bytes_to_read > MAX_INDEX - 1 )
    {
        size_t available_bytes_to_right = MAX_INDEX - offset;
        memcpy(out,ptr + offset, available_bytes_to_right);
        bytes_to_read -= available_bytes_to_right;
        out           += available_bytes_to_right;
        offset         = 0;
        rb->start      = 0;
    }
    memcpy(out,ptr + offset, bytes_to_read);
    
    rb->current_size -= out_size;
    rb->start         = offset + bytes_to_read;

    return bytes_to_read;
}
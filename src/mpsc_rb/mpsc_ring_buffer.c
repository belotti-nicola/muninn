#include <internal/mpsc_ring_buffer.h>
#include <string.h>

bool mpsc_rb_setup(mpsc_ring_buffer *rb, uint8_t *data, size_t data_size)
{
    if (data == NULL || rb == NULL) return false;

    if (data_size == 0) return false;
    
    if ((data_size & (data_size - 1)) != 0) return false;

    rb->data = data;
    rb->data_size = data_size;
    atomic_store(&rb->start, 0);
    atomic_store(&rb->end, 0);

    return true;
}

bool mpsc_rb_push(mpsc_ring_buffer *rb, uint8_t *buffer, size_t buffer_size)
{
    if (rb == NULL || buffer == NULL) return false;
    
    if (buffer_size == 0) return false;
    
    size_t start;
    size_t end;
    size_t new_end;
    size_t max = rb->data_size;

    do
    {
        start = atomic_load_explicit(&rb->start, memory_order_acquire);
        end   = atomic_load_explicit(&rb->end, memory_order_relaxed);

        size_t used = (end >= start) ? (end - start) : (max - (start - end));
        size_t free_space = max - used;

        if (buffer_size >= free_space) 
        {
            return false; 
        }

        new_end = (end + buffer_size) % max;
    }
    while (!atomic_compare_exchange_strong(&rb->end, &end, new_end)); 
    
    size_t bytes_to_copy_first = (end + buffer_size > max) ? (max - end) : buffer_size;
    memcpy(rb->data + end, buffer, bytes_to_copy_first);

    if (bytes_to_copy_first < buffer_size)
    {
        memcpy(rb->data, buffer + bytes_to_copy_first, buffer_size - bytes_to_copy_first);
    }

    return true;
}

bool mpsc_rb_peek(mpsc_ring_buffer *rb, uint8_t *data, size_t data_size)
{
    if (rb == NULL || data == NULL) return false;
    if (data_size == 0) return false;

    size_t max   = rb->data_size;
    size_t start = atomic_load_explicit(&rb->start, memory_order_acquire);
    size_t end   = atomic_load_explicit(&rb->end, memory_order_relaxed);

    size_t used = (end >= start) ? (end - start) : (max - (start - end));
    if (data_size > used) return false; // Dati insufficienti

    size_t bytes_to_copy_first = (start + data_size > max) ? (max - start) : data_size;
    memcpy(data, rb->data + start, bytes_to_copy_first);

    if (bytes_to_copy_first < data_size)
    {
        memcpy(data + bytes_to_copy_first, rb->data, data_size - bytes_to_copy_first);
    }

    return true;
}

bool mpsc_rb_advance(mpsc_ring_buffer *rb, size_t data_size)
{
    if (rb == NULL) return false;
    if (data_size == 0) return false;

    atomic_fetch_add_explicit(&rb->start, data_size, memory_order_release);
    return true;
}
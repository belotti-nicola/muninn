#include <internal/mpsc_ring_buffer.h>
#include <string.h>

#include <sched.h>
#include <limits.h>

#define B4_YIELD 10000

size_t internal_compute_current_size(mpsc_ring_buffer *sb, size_t start, size_t end)
{
    // since :
    //      1. consumers waits for producers work
    //      2. indices are growing numbers
    // 
    // ===> concrete size is the differenze of the indices.

    return end - start;
}


bool mpsc_rb_init(mpsc_ring_buffer *rb, uint8_t *buffer, size_t buffer_size)
{
    if (rb == NULL || buffer == NULL || buffer_size == 0) return false;

    if ( sb_init( &rb->buffer, buffer, buffer_size ) == false ) 
    {
        return false;
    }

    atomic_init( &rb->producers_index  , 0);
    atomic_init( &rb->producers_commit , 0);
    atomic_init( &rb->consumer_index   , 0);

    rb->futex_empty = 0;
    rb->futex_full  = 1;
    
    return true;
}

bool mpsc_rb_push(mpsc_ring_buffer *sb, uint8_t *buff, size_t buff_size)
{
    if (sb == NULL || buff == NULL || buff_size == 0) return false;

    size_t max_size = sb->buffer.buffer_size;
    if (max_size == 0 || buff_size > max_size) return false;

    uint64_t producers_i,consumer_i,current_size;

    size_t desired_i,desired_c;

    // ====================================
    // ======== RESERVATION LOOP ==========
    // ====================================
    do
    {
        producers_i = atomic_load(&sb->producers_index);
        consumer_i  = atomic_load(&sb->consumer_index);

        if ( internal_compute_current_size(sb,consumer_i, producers_i) + buff_size > max_size )
        {           
            futex_t expected_futex = atomic_load(&sb->futex_full);
           
            producers_i = atomic_load(&sb->producers_index);
            consumer_i  = atomic_load(&sb->consumer_index);
            if ( internal_compute_current_size(sb, consumer_i, producers_i) + buff_size <= max_size )
            {
                continue; 
            }

            futex_wait(&sb->futex_full, expected_futex);
            continue;
        }

        desired_i = producers_i + buff_size;

    } while ( atomic_compare_exchange_strong( &sb->producers_index, &producers_i, desired_i ) == false ) ;    
    
    // ====================================
    // ======== COPY INSTRUCTION ==========
    // ====================================
    if ( sb_copy_in(&sb->buffer, producers_i % max_size, buff, buff_size) == false ) 
    {
        return false;
    }

    // ====================================
    // ========== COMMIT PHASE ============
    // ====================================
    int commit_i = 0;
    while ( atomic_load_explicit(&sb->producers_commit, memory_order_acquire) != producers_i )
    {
        if (commit_i < 100) 
        {
            commit_i++;
            sched_yield();
        }
        else
        {
            sched_yield();
            commit_i = 0;
        }
    }
    atomic_store_explicit(&sb->producers_commit, producers_i + buff_size, memory_order_release);

    // ====================================
    // ========== UPDATE FUTEX ============
    // ====================================
    atomic_fetch_add(&sb->futex_empty, 1);

    futex_wake(&sb->futex_empty, INT_MAX);

    return true;
}

bool mpsc_rb_peek(mpsc_ring_buffer *sb, uint8_t *out, size_t size)
{
        if (  sb == NULL ) return false;
 
    if ( out == NULL || size == 0 ) return false;

    size_t max_size = sb->buffer.buffer_size;
    if (max_size == 0 || size > max_size) return false;

    uint64_t consumer_i;
    uint64_t producers_i;

    while (true)
    {
        consumer_i  = atomic_load_explicit(&sb->consumer_index,   memory_order_relaxed);
        producers_i = atomic_load_explicit(&sb->producers_commit, memory_order_acquire);

        if (internal_compute_current_size(sb, consumer_i, producers_i) >= size)
        {
            break;
        }

        futex_t futex_status = atomic_load(&sb->futex_empty);
        
        // avoid lost wake ups.
        consumer_i  = atomic_load_explicit(&sb->consumer_index,   memory_order_relaxed);
        producers_i = atomic_load_explicit(&sb->producers_commit, memory_order_acquire);
        if (internal_compute_current_size(sb, consumer_i, producers_i) >= size)
        {
            break;
        }

        futex_wait(&sb->futex_empty, futex_status);
    }

    uint64_t offset = consumer_i % max_size;

    if ( sb_copy_out(&sb->buffer, offset, out, size) == false )
    {
        return false;
    }

    return true;
}

bool mpsc_rb_advance(mpsc_ring_buffer *sb, size_t size)
{
    if (  sb == NULL ) return false;
 
    if ( size == 0 ) return false;

    size_t max_size = sb->buffer.buffer_size;
    if (max_size == 0 || size > max_size) return false;

    atomic_fetch_add_explicit(&sb->consumer_index, size, memory_order_release);

    atomic_store_explicit(&sb->futex_full, atomic_load(&sb->futex_full) + 1, memory_order_release);

    futex_wake(&sb->futex_full, INT_MAX);

    return true;

}
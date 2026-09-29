#ifndef MPSC_RING_BUFFER_H
#define MPSC_RING_BUFFER_H

#include <internal/safe_buffer.h>
#include <internal/futex.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__)
    #include <immintrin.h>
    #define CPU_PAUSE() _mm_pause()
#elif defined(__aarch64__) || defined(_ARM_)
    #define CPU_PAUSE() __asm__ __volatile__("yield" ::: "memory")
#else
    #define CPU_PAUSE() ((void)0) // Fallback vuoto se architettura sconosciuta
#endif

typedef struct mpsc_ring_buffer
{
    safe_buffer_t buffer;

    uint64_t  producers_index;
    uint64_t  producers_commit;
    
    uint64_t  consumer_index;

    futex_t   futex_empty;
    futex_t   futex_full; 

} mpsc_ring_buffer;

bool mpsc_rb_init(mpsc_ring_buffer *rb, uint8_t *buffer, size_t buffer_size);

bool mpsc_rb_push(mpsc_ring_buffer *sb,uint8_t *buff,size_t buff_size);

bool mpsc_rb_peek(mpsc_ring_buffer *sb, uint8_t *buff, size_t buff_size);
bool mpsc_rb_advance(mpsc_ring_buffer *sb, size_t buff_size);



#endif
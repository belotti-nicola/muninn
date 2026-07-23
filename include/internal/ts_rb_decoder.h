#ifndef TS_RB_MM_DECODER_H
#define TS_RB_MM_DECODER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct mm_decoder_t
{
    uint8_t *buffer;
    size_t   buffer_size;
    size_t   offset;
    size_t   current_size;

} mm_decoder_t;

void decoder_setup(mm_decoder_t *decoder,uint8_t *buffer,size_t buffer_size);

bool decode_u8(mm_decoder_t *decoder,uint8_t *value);
bool decode_u16(mm_decoder_t *decoder,uint16_t *value);
bool decode_u32(mm_decoder_t *decoder,uint32_t *value);
bool decode_u64(mm_decoder_t *decoder,uint64_t *value);
bool decode_bytes(mm_decoder_t *decoder,uint8_t *values,size_t values_size);

#endif
#ifndef TS_RB_MM_ENCODER_H
#define TS_RB_MM_ENCODER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct mm_encoder_t
{
    uint8_t *buffer;
    size_t   buffer_size;
    size_t   offset;
    size_t   current_size;

} mm_encoder_t;

void encoder_setup(mm_encoder_t *encoder,uint8_t *buffer,size_t buffer_size);

bool encode_u8(mm_encoder_t *encoder,uint8_t value);
bool encode_u16(mm_encoder_t *encoder,uint16_t value);
bool encode_u32(mm_encoder_t *encoder,uint32_t value);
bool encode_u64(mm_encoder_t *encoder,uint64_t value);
bool encode_bytes(mm_encoder_t *encoder,uint8_t *values,size_t values_size);

#endif
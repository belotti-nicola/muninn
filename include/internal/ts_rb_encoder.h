#ifndef TS_RB_ENCODER_H
#define TS_RB_ENCODER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct encoder_t
{
    uint8_t *buffer;
    size_t   buffer_size;
    size_t   offset;
    size_t   current_size;

} encoder_t;

void encoder_setup(encoder_t *encoder,uint8_t *buffer,size_t buffer_size);

bool encode_u8(encoder_t *encoder,uint8_t value);
bool encode_u16(encoder_t *encoder,uint16_t value);
bool encode_u32(encoder_t *encoder,uint32_t value);
bool encode_u64(encoder_t *encoder,uint64_t value);
bool encode_bytes(encoder_t *encoder,uint8_t *values,size_t values_size);

#endif
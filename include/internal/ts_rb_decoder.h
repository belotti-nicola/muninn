#ifndef TS_RB_DECODER_H
#define TS_RB_DECODER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct decoder_t
{
    uint8_t *buffer;
    size_t   buffer_size;
    size_t   offset;

} decoder_t;

void decoder_setup(decoder_t *decoder,uint8_t *buffer,size_t buffer_size);

bool decode_u8(decoder_t *decoder,uint8_t *value);
bool decode_u16(decoder_t *decoder,uint16_t *value);
bool decode_u32(decoder_t *decoder,uint32_t *value);
bool decode_u64(decoder_t *decoder,uint64_t *value);
bool decode_bytes(decoder_t *decoder,uint8_t *values,size_t values_size);

#endif
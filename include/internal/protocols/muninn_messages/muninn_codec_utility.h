#ifndef CODEC_UTILITY_H
#define CODEC_UTILITY_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct mcu_encoder_t
{
    uint8_t *buffer;
    size_t   buffer_size;
    size_t   offset;
    size_t   current_size;

} mcu_encoder_t;

typedef struct mcu_decoder_t
{
    const uint8_t *buffer;
    size_t         buffer_size;
    size_t         offset;
    size_t         current_size;

} mcu_decoder_t;

void mcu_encoder_setup(mcu_encoder_t *encoder,uint8_t *data,size_t data_size);
bool mcu_encode_u8(    mcu_encoder_t *encoder,        uint8_t    value);
bool mcu_encode_u16(   mcu_encoder_t *encoder,        uint16_t   value);
bool mcu_encode_u32(   mcu_encoder_t *encoder,        uint32_t   value);
bool mcu_encode_u64(   mcu_encoder_t *encoder,        uint64_t   value);
bool mcu_encode_bytes( mcu_encoder_t *encoder,  const uint8_t   *values, size_t values_size);


void mcu_decoder_setup(mcu_decoder_t *decoder,const uint8_t *buffer,size_t buffer_size);
bool mcu_decode_u8(mcu_decoder_t *decoder,uint8_t *value);
bool mcu_decode_u16(mcu_decoder_t *decoder,uint16_t *value);
bool mcu_decode_u32(mcu_decoder_t *decoder,uint32_t *value);
bool mcu_decode_u64(mcu_decoder_t *decoder,uint64_t *value);
bool mcu_decode_bytes(mcu_decoder_t *decoder,uint8_t *values,size_t values_size);

#endif
#ifndef CODEC_UTILITY_H
#define CODEC_UTILITY_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct encoder_t
{
    uint8_t *buffer;
    size_t   buffer_size;
    size_t   offset;
    size_t   current_size;

} encoder_t;

typedef struct decoder_t
{
    const uint8_t *buffer;
    size_t         buffer_size;
    size_t         offset;
    size_t         current_size;

} decoder_t;

void mc_encoder_setup(encoder_t *encoder,uint8_t *data,size_t data_size);
bool mc_encode_u8(    encoder_t *encoder,        uint8_t    value);
bool mc_encode_u16(   encoder_t *encoder,        uint16_t   value);
bool mc_encode_u32(   encoder_t *encoder,        uint32_t   value);
bool mc_encode_u64(   encoder_t *encoder,        uint64_t   value);
bool mc_encode_bytes( encoder_t *encoder,  const uint8_t   *values, size_t values_size);


void mc_decoder_setup(decoder_t *decoder,const uint8_t *buffer,size_t buffer_size);
bool mc_decode_u8(decoder_t *decoder,uint8_t *value);
bool mc_decode_u16(decoder_t *decoder,uint16_t *value);
bool mc_decode_u32(decoder_t *decoder,uint32_t *value);
bool mc_decode_u64(decoder_t *decoder,uint64_t *value);
bool mc_decode_bytes(decoder_t *decoder,uint8_t *values,size_t values_size);

#endif
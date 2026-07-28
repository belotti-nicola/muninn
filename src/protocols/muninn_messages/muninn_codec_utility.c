#include <internal/protocols/muninn_messages/muninn_codec_utility.h>

#include <string.h>

void mcu_encoder_setup(mcu_encoder_t *encoder,uint8_t *buffer,size_t buffer_size)
{
    encoder->buffer       = buffer;
    encoder->buffer_size  = buffer_size;
    encoder->current_size = 0;
    encoder->offset       = 0;
}

bool mcu_encode_u8(mcu_encoder_t *encoder,uint8_t value)
{
    if (encoder == NULL) return false;

    if (encoder->current_size + sizeof(uint8_t) > encoder->buffer_size) return false;

    size_t    index = encoder->offset;
    uint8_t *target = encoder->buffer + index;

    memcpy(target,&value,sizeof(uint8_t));
    encoder->offset       += sizeof(uint8_t);
    encoder->current_size += sizeof(uint8_t);
    
    return true;
}

bool mcu_encode_u16(mcu_encoder_t *encoder,uint16_t value)
{
    if (encoder == NULL)
        return false;

    if (encoder->current_size + sizeof(uint16_t) > encoder->buffer_size)
        return false;

    size_t    index = encoder->offset;
    uint8_t *target = encoder->buffer + index;

    memcpy(target,&value,sizeof(uint16_t));
    encoder->offset      += sizeof(uint16_t);
    encoder->current_size += sizeof(uint16_t);
    return true;
}

bool mcu_encode_u32(mcu_encoder_t *encoder,uint32_t value)
{
    if (encoder == NULL)
        return false;

    if (encoder->current_size + sizeof(uint32_t) > encoder->buffer_size)
        return false;

    size_t    index = encoder->offset;
    uint8_t *target = encoder->buffer + index;

    memcpy(target,&value,sizeof(uint32_t));
    encoder->offset      += sizeof(uint32_t);
    encoder->current_size += sizeof(uint32_t);
    return true;
}

bool mcu_encode_u64(mcu_encoder_t *encoder,uint64_t value)
{
    if (encoder == NULL)
        return false;

    if (encoder->current_size + sizeof(uint64_t) > encoder->buffer_size)
        return false;

    size_t    index = encoder->offset;
    uint8_t *target = encoder->buffer + index;
    size_t   bytes  = sizeof(uint64_t);

    memcpy(target,&value,sizeof(uint64_t));
    encoder->offset      += sizeof(uint64_t);
    encoder->current_size += sizeof(uint64_t);
    return true;
}

bool mcu_encode_bytes(mcu_encoder_t *encoder,const uint8_t *values,size_t values_size)
{
    if (encoder == NULL)
        return false;

    if (encoder->current_size + values_size > encoder->buffer_size)
        return false;

    size_t    index = encoder->offset;
    uint8_t *target = encoder->buffer + index;

    memcpy(target,values,values_size);
    encoder->offset      += values_size;
    encoder->current_size += values_size;
    return true;
}

void mcu_decoder_setup(mcu_decoder_t *decoder, const uint8_t *buffer, size_t buffer_size)
{
    decoder->buffer       = buffer;
    decoder->buffer_size  = buffer_size;
    decoder->offset       = 0;
    decoder->current_size = 0;
}

bool mcu_decode_u8(mcu_decoder_t *decoder,uint8_t *value)
{
    if(decoder == NULL || value == NULL ) 
    {
        return false;
    }
    if(decoder->current_size + sizeof(uint8_t) > decoder->buffer_size )
    {
        return false;
    }

    size_t   offset = decoder->offset;
    const uint8_t *target = decoder->buffer + offset;
    *value = *target;

    decoder->offset       += 1;
    decoder->current_size += 1;
    return true;
}

bool mcu_decode_u16(mcu_decoder_t *decoder,uint16_t *value)
{
    if(decoder == NULL || value == NULL) 
    {
        return false;
    }
    if(decoder->current_size + sizeof(uint16_t) > decoder->buffer_size )
    {
        return false;
    }
    
    size_t   offset = decoder->offset;
    const uint8_t *target = decoder->buffer + offset;
    size_t   bytes  = sizeof(uint16_t);

    memcpy(value,target,bytes);

    decoder->offset       += bytes;
    decoder->current_size += bytes;
    return true;
}
bool mcu_decode_u32(mcu_decoder_t *decoder,uint32_t *value)
{
    if(decoder == NULL || value == NULL) 
    {
        return false;
    }
    if(decoder->current_size + sizeof(uint32_t) > decoder->buffer_size )
    {
        return false;
    }
    
    size_t   offset = decoder->offset;
    const uint8_t *target = decoder->buffer + offset;
    size_t   bytes  = sizeof(uint32_t);

    memcpy(value,target,bytes);
    
    decoder->offset       += bytes;
    decoder->current_size += bytes;
    return true;
}
bool mcu_decode_u64(mcu_decoder_t *decoder,uint64_t *value)
{
    if(decoder == NULL || value == NULL) 
    {
        return false;
    }
    if(decoder->current_size + sizeof(uint64_t) > decoder->buffer_size )
    {
        return false;
    }
    
    size_t   offset = decoder->offset;
    const uint8_t *target = decoder->buffer + offset;
    size_t   bytes  = sizeof(uint64_t);

    memcpy(value,target,bytes);
    
    decoder->offset       += bytes;
    decoder->current_size += bytes;
    return true;
}
bool mcu_decode_bytes(mcu_decoder_t *decoder,uint8_t *values,size_t values_size)
{
    if(decoder == NULL || values == NULL) 
    {
        return false;
    }
    if(decoder->current_size + values_size > decoder->buffer_size )
    {
        return false;
    }
    
    size_t   offset = decoder->offset;
    const uint8_t *target = decoder->buffer + offset;

    memcpy(values,target,values_size);
    
    decoder->offset       += values_size;
    decoder->current_size += values_size;
    return true;
}
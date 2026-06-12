#include <internal/ts_rb_encoder.h>
#include <string.h>

void encoder_setup(encoder_t *encoder,uint8_t *buffer,size_t buffer_size)
{
    encoder->buffer       = buffer;
    encoder->buffer_size  = buffer_size;
    encoder->current_size = 0;
    encoder->offset       = 0;
}

bool encode_u8(encoder_t *encoder,uint8_t value)
{
    if (encoder == NULL)
        return false;

    if (encoder->current_size + sizeof(uint32_t) > encoder->buffer_size)
        return false;

    size_t    index = encoder->offset;
    uint8_t *target = encoder->buffer + index;

    memcpy(target,&value,sizeof(uint8_t));
    encoder->offset      += sizeof(uint8_t);
    encoder->current_size += sizeof(uint8_t);
    return true;
}

bool encode_u16(encoder_t *encoder,uint16_t value)
{
    if (encoder == NULL)
        return false;

    if (encoder->current_size + sizeof(uint32_t) > encoder->buffer_size)
        return false;

    size_t    index = encoder->offset;
    uint8_t *target = encoder->buffer + index;

    memcpy(target,&value,sizeof(uint16_t));
    encoder->offset      += sizeof(uint16_t);
    encoder->current_size += sizeof(uint16_t);
    return true;
}

bool encode_u32(encoder_t *encoder,uint32_t value)
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

bool encode_u64(encoder_t *encoder,uint64_t value)
{
    if (encoder == NULL)
        return false;

    if (encoder->current_size + sizeof(uint32_t) > encoder->buffer_size)
        return false;

    size_t    index = encoder->offset;
    uint8_t *target = encoder->buffer + index;
    size_t   bytes  = sizeof(uint64_t);

    memcpy(target,&value,sizeof(uint64_t));
    encoder->offset      += sizeof(uint64_t);
    encoder->current_size += sizeof(uint64_t);
    return true;
}

bool encode_bytes(encoder_t *encoder,uint8_t *values,size_t values_size)
{
    if (encoder == NULL)
        return false;

    if (encoder->current_size + sizeof(uint32_t) > encoder->buffer_size)
        return false;

    size_t    index = encoder->offset;
    uint8_t *target = encoder->buffer + index;

    memcpy(target,values,values_size);
    encoder->offset      += values_size;
    encoder->current_size += values_size;
    return true;
}
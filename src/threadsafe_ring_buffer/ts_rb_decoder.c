#include <internal/ts_rb_decoder.h>
#include <string.h>

void decoder_setup(decoder_t *decoder,uint8_t *buffer,size_t buffer_size)
{
    decoder->buffer       = buffer;
    decoder->buffer_size  = buffer_size;
    decoder->offset       = 0;
    decoder->current_size = 0;
}

bool decode_u8(decoder_t *decoder,uint8_t *value)
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
    uint8_t *target = decoder->buffer + offset;
    *value = *target;

    decoder->offset       += 1;
    decoder->current_size += 1;
    return true;
}

bool decode_u16(decoder_t *decoder,uint16_t *value)
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
    uint8_t *target = decoder->buffer + offset;
    size_t   bytes  = sizeof(uint16_t);

    memcpy(value,target,bytes);

    decoder->offset       += bytes;
    decoder->current_size += 1;
    return true;
}
bool decode_u32(decoder_t *decoder,uint32_t *value)
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
    uint8_t *target = decoder->buffer + offset;
    size_t   bytes  = sizeof(uint32_t);

    memcpy(value,target,bytes);
    
    decoder->offset       += bytes;
    decoder->current_size += bytes;
    return true;
}
bool decode_u64(decoder_t *decoder,uint64_t *value)
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
    uint8_t *target = decoder->buffer + offset;
    size_t   bytes  = sizeof(uint64_t);

    memcpy(value,target,bytes);
    
    decoder->offset       += bytes;
    decoder->current_size += bytes;
    return true;
}
bool decode_bytes(decoder_t *decoder,uint8_t *values,size_t values_size)
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
    uint8_t *target = decoder->buffer + offset;

    memcpy(values,target,values_size);
    
    decoder->offset       += values_size;
    decoder->current_size += values_size;
    return true;
}
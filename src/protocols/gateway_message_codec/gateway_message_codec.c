#include <internal/protocols/gateway_message_codec/gateway_message_codec.h>

#include <internal/muninn_message.h>
#include <string.h>

bool gateway_message_codec_prependable_size(
    size_t         in_size, 
    uint8_t       *out, 
    size_t         out_size,
    size_t        *bytes_written)
{
    if ( in_size == 0 || out == NULL) return false;

    if (in_size > UINT16_MAX) return false;

    size_t total_size = sizeof(uint16_t) + in_size;
    if (out_size < total_size) return false;

    uint16_t len16 = (uint16_t)in_size;
    memcpy(out, &len16, sizeof(uint16_t));

    if (bytes_written != NULL) 
    {
        *bytes_written = total_size;
    }

    return true;
}

bool gateway_message_codec_decode_message_size(
    uint8_t *buffer,
    size_t  *offset)
{
    if ( buffer == NULL || offset == NULL ) return false;
    
    memcpy(offset,buffer,sizeof(uint16_t));

    return true;
}
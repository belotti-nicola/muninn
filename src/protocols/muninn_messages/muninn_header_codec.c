#include <internal/protocols/muninn_messages/muninn_header_codec.h>

#include <internal/protocols/muninn_messages/muninn_message.h>
#include <internal/protocols/muninn_messages/muninn_codec_utility.h>


bool muninn_header_encode(muninn_header *muninn_header, uint8_t *out, size_t *out_size)
{
    if(muninn_header == NULL || out == NULL || out_size == NULL) return false;

    if(*out_size < sizeof(*muninn_header)) return false;

    mcu_encoder_t encoder = {0};
    mcu_encoder_setup(&encoder,out,*out_size);

    if(mcu_encode_u16(&encoder,muninn_header->payload_len) == false) return false;

    *out_size = encoder.current_size;

    return true;
}

bool muninn_header_decode(const uint8_t *in, size_t in_size, muninn_header *muninn_header)
{
    if(in == NULL || muninn_header == NULL ) return false;

    if(in_size < sizeof(*muninn_header)) return false;

    mcu_decoder_t decoder = {0};
    mcu_decoder_setup(&decoder,in,in_size);

    if(mcu_decode_u16(&decoder,&muninn_header->payload_len) == false) return false;

    return true;
}
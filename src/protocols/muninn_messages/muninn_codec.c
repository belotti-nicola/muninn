#include <internal/protocols/muninn_messages/muninn_codec.h>

#include <internal/protocols/muninn_messages/muninn_message.h>
#include <internal/protocols/muninn_messages/muninn_message_mask.h>
#include <internal/protocols/muninn_messages/muninn_payload_codec.h>
#include <internal/protocols/muninn_messages/muninn_header_codec.h>


bool muninn_messages_encode(muninn_message *mm, uint8_t *out, size_t *out_size)
{
    if(mm == NULL || out == NULL || out_size == NULL || *out_size == 0) return false;

    size_t offset = sizeof(muninn_header);
    if(*out_size < offset) return false;

    uint8_t     *target = offset + out;
    size_t payload_size = *out_size - offset;
    if(muninn_payload_encode(mm->payload,target,&payload_size) == false)
    {
        return false;
    }

    size_t header_size = sizeof(muninn_header);
    muninn_header mh = {0};
    mh.payload_len = payload_size;
    if(muninn_header_encode(&mh,out,&header_size) == false)
    {
        return false;
    }

    *out_size = payload_size + header_size;

    return true;
}

bool muninn_messages_decode(const uint8_t *in, size_t in_size, muninn_message *mm)
{
    if(in == NULL || mm == NULL || in_size == 0) return false;

    size_t header_size = sizeof(muninn_header);
    if(muninn_header_decode(in,header_size,mm->header) == false)
    {
        return false;
    }

    size_t offset = header_size;
    const uint8_t *target = in+offset;
    size_t bytes_available = mm->header->payload_len;
    if(muninn_payload_decode(target,bytes_available,mm->payload) == false)
    {
        return false;
    }

    return true;
}
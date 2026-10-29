#ifndef GATEWAY_MESSAGE_CODEC_H
#define GATEWAY_MESSAGE_CODEC_H

#include <internal/muninn_message.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

bool gateway_message_codec_prependable_size(
    size_t         in_size, 
    uint8_t       *out, 
    size_t         out_size,
    size_t        *bytes_written
);

bool gateway_message_codec_decode_message_size(
    uint8_t       *buffer,
    size_t        *offset
);

#endif
#ifndef MUNINN_ENCODED_MESSAGES_CODEC_H
#define MUNINN_ENCODED_MESSAGES_CODEC_H


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <internal/protocols/muninn_encoded_messages/muninn_encoded_message_mask.h>

typedef enum   muninn_message_mask muninn_message_mask;
typedef struct muninn_message_t      muninn_message_t;

bool muninn_messages_encode(
    muninn_message_t *mm,
    muninn_message_mask mask,
    uint8_t *out,
    size_t   out_size,
    size_t *written_bytes
);

bool muninn_messages_decode(
    const uint8_t *in,
    size_t in_size,
    muninn_message_t *mm
);


#endif
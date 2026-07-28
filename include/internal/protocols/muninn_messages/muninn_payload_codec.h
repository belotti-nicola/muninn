#ifndef MUNINN_PAYLOAD_CODEC_H
#define MUNINN_PAYLOAD_CODEC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct muninn_payload      muninn_payload;
typedef enum   muninn_message_mask muninn_message_mask;

bool muninn_payload_encode(muninn_payload *muninn_payload, muninn_message_mask mmm, uint8_t *out, size_t *out_size);
bool muninn_payload_decode(const uint8_t *in, size_t in_size, muninn_payload *muninn_payload);


#endif
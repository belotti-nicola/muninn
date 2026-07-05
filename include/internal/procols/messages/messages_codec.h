#ifndef MESSAGES_CODEC_H
#define MESSAGES_CODEC_H

#include "encoded_data_mask.h"

#include <stdint.h>
#include <stddef.h>

void mm_encode(
    ENCONDED_DATA_MASK mask,
    uint8_t *out, size_t out_size
);

void mm_decode(
    const uint8_t *in,  size_t in_size,
    uint8_t       *out, size_t out_max_size, size_t *out_size
);

#endif
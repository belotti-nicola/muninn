#ifndef MESSAGES_CODEC_H
#define MESSAGES_CODEC_H

#include "encoded_data.h"
#include "encoded_data_mask.h"

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

bool mm_encode(
    const ENCONDED_DATA_MASK *mask,
    const mnn_data_t         *data,
    uint8_t *out, size_t out_size,
    size_t *encode_bytes
);

bool mm_decode(
    mnn_data_t    *out,
    const uint8_t *in, size_t  in_size,
    size_t *decoded_bytes
);

#endif
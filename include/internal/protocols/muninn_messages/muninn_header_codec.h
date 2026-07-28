#ifndef MUNINN_HEADER_CODEC_H
#define MUNINN_HEADER_CODEC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct muninn_header       muninn_header;

bool muninn_header_encode(muninn_header *muninn_header, uint8_t *out, size_t *out_size);
bool muninn_header_decode(const uint8_t *in, size_t in_size, muninn_header *muninn_header);


#endif
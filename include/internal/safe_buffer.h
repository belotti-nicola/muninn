#ifndef SAFE_BUFFER_H
#define SAFE_BUFFER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct safe_buffer_t
{
    uint8_t *buffer;
    size_t   buffer_size;

} safe_buffer_t;

bool sb_init(safe_buffer_t *sb, uint8_t *buffer, size_t buffer_size);
bool sb_copy_out(const safe_buffer_t *sb, size_t offset, uint8_t *dest, size_t len);
bool sb_copy_in(safe_buffer_t *sb, size_t offset, const uint8_t *src, size_t len);

#endif
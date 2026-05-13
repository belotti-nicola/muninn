#ifndef XZ_WRAPPER_H
#define XZ_WRAPPER_H

#include <stddef.h>

int xz_compress(
    const void *src,
    size_t src_size,
    void **dst,
    size_t *dst_size,
    int level
);

int xz_decompress(
    const void *src,
    size_t src_size,
    void **dst,
    size_t *dst_size
);



#endif
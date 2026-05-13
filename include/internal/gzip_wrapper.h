#ifndef GZIP_WRAPPER_H
#define GZIP_WRAPPER_H

#include <stddef.h>


int gzip_compress(
    const void *src,
    size_t src_size,
    void **dst,
    size_t *dst_size,
    int level
);

int gzip_decompress(
    const void *src,
    size_t src_size,
    void **dst,
    size_t *dst_size,
    int level
);

#endif
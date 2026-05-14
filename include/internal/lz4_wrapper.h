#ifndef LZ4_WRAPPER_H
#define LZ4_WRAPPER_H

#include <stddef.h>

int lz4_compress(
    const void *src,
    size_t      src_size,
    void       *dst,
    size_t      dst_size,
    size_t     *written,
    int         level
);

int lz4_decompress(
    const void *src,
    size_t      src_size,
    void       *dst,
    size_t      dst_size,
    size_t     *written
);

int lz4_compress_file(
    const char *src_path,
    const char *dst_path,
    int level
);



#endif
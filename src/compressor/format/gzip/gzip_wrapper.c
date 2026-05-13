#include <internal/gzip_wrapper.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <zlib.h>

int gzip_compress(
    const void *src,
    size_t src_size,
    void **dst,
    size_t *dst_size,
    int level)
{
    uLong bound = compressBound(src_size);

    void *out = malloc(bound);
    if (!out)
        return -1;

    z_stream zs;
    memset(&zs, 0, sizeof(zs));

    if (deflateInit2(
            &zs,
            level,
            Z_DEFLATED,
            15 + 16, // gzip header
            8,
            Z_DEFAULT_STRATEGY) != Z_OK)
    {
        free(out);
        return -2;
    }

    zs.next_in = (Bytef *)src;
    zs.avail_in = src_size;

    zs.next_out = out;
    zs.avail_out = bound;

    int ret = deflate(&zs, Z_FINISH);

    if (ret != Z_STREAM_END) {
        deflateEnd(&zs);
        free(out);
        return -3;
    }

    *dst_size = zs.total_out;
    *dst = out;

    deflateEnd(&zs);

    return 0;
}

int gzip_decompress(
    const void *src,
    size_t src_size,
    void **dst,
    size_t *dst_size,
    int level
)
{
    return 0;
}
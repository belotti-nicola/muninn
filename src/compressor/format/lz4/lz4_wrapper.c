#include <internal/lz4_wrapper.h>

#include <lz4.h>
#include <lz4hc.h>
#include <stdlib.h>

int lz4_compress(
    const void *src,
    size_t src_size,
    void **dst,
    size_t *dst_size,
    int level)
{
    int bound = LZ4_compressBound((int)src_size);

    void *out = malloc(bound);
    if (!out)
        return -1;

    int written = LZ4_compress_HC(
        (const char *)src,
        (char *)out,
        (int)src_size,
        bound,
        level);

    if (written <= 0) {
        free(out);
        return -2;
    }

    *dst = out;
    *dst_size = written;

    return 0;
}

int lz4_decompress(
    const void *src,
    size_t src_size,
    void **dst,
    size_t *dst_size
)
{
    void *out = malloc(*dst_size);
    if (!out)
        return -1;

    int ret = LZ4_decompress_safe(
        (const char *)src,
        (char *)out,
        (int)src_size,
        (int)*dst_size);

    if (ret < 0) {
        free(out);
        return -2;
    }

    *dst = out;
    *dst_size = ret;

    return 0;
}
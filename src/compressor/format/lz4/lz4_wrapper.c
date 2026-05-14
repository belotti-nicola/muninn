#include <internal/lz4_wrapper.h>

#include <lz4.h>
#include <lz4hc.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include <limits.h>

int lz4_compress(
    const void *src,
    size_t src_size,
    void *dst,
    size_t dst_capacity,
    size_t *written,
    int level)
{
    if (!src || !dst || !written)
        return -1;

    if (src_size > INT_MAX)
        return -2;

    if (dst_capacity > INT_MAX)
        return -3;

    int bound = LZ4_compressBound((int)src_size);

    if ((int)dst_capacity < bound)
        return -4;

    if (level < LZ4HC_CLEVEL_MIN)
        level = LZ4HC_CLEVEL_MIN;

    if (level > LZ4HC_CLEVEL_MAX)
        level = LZ4HC_CLEVEL_MAX;

    int ret = LZ4_compress_HC(
        (const char *)src,
        (char *)dst,
        (int)src_size,
        (int)dst_capacity,
        level);

    if (ret <= 0)
        return -5;

    *written = (size_t)ret;

    return 0;
}

int lz4_decompress(
    const void *src,
    size_t src_size,

    void *dst,
    size_t dst_capacity,

    size_t *written)
{
    if (!src || !dst || !written)
        return -1;

    if (src_size > INT_MAX)
        return -2;

    if (dst_capacity > INT_MAX)
        return -3;

    int ret = LZ4_decompress_safe(
        (const char *)src,
        (char *)dst,
        (int)src_size,
        (int)dst_capacity);

    if (ret < 0)
        return -4;

    *written = (size_t)ret;

    return 0;
}

int lz4_compress_file(
    const char *src_path,
    const char *dst_path,
    int level)
{
    FILE *in = fopen(src_path, "rb");
    if (!in)
        return -1;

    FILE *out = fopen(dst_path, "wb");
    if (!out) {
        fclose(in);
        return -2;
    }

    static uint8_t input[1024 * 1024];

    static uint8_t compressed[
        LZ4_COMPRESSBOUND(1024 * 1024)
    ];

    size_t read = fread(input, 1, sizeof(input), in);

    if (ferror(in)) {
        fclose(in);
        fclose(out);
        return -3;
    }

    size_t written;

    int rc = lz4_compress(
        input,
        read,

        compressed,
        sizeof(compressed),

        &written,

        level);

    if (rc != 0) {
        fclose(in);
        fclose(out);
        return -4;
    }

    typedef struct {
        uint32_t original_size;
        uint32_t compressed_size;
    } Header;

    Header hdr = {
        .original_size   = (uint32_t)read,
        .compressed_size = (uint32_t)written
    };

    fwrite(&hdr, sizeof(hdr), 1, out);
    fwrite(compressed, written, 1, out);

    fclose(in);
    fclose(out);

    return 0;
}
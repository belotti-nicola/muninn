#include <internal/lz4_wrapper.h>

#include <lz4frame.h>
#include <lz4hc.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

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

    size_t bound = LZ4F_compressFrameBound(src_size, NULL);

    if (dst_capacity < bound)
        return -4;

    LZ4F_preferences_t prefs = {0};
    
    if (level < 1) level = 1;
    if (level > LZ4HC_CLEVEL_MAX) level = LZ4HC_CLEVEL_MAX;
    
    prefs.compressionLevel = level;

    size_t ret = LZ4F_compressFrame(
        dst, dst_capacity,
        src, src_size,
        &prefs
    );

    if (LZ4F_isError(ret))
        return -5;

    *written = ret;

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

    LZ4F_dctx* dctx;
    LZ4F_errorCode_t err = LZ4F_createDecompressionContext(&dctx, LZ4F_VERSION);
    if (LZ4F_isError(err)) {
        return -2;
    }

    size_t dst_size = dst_capacity;
    size_t src_sz = src_size;

    size_t ret = LZ4F_decompress(dctx, dst, &dst_size, src, &src_sz, NULL);

    LZ4F_freeDecompressionContext(dctx);

    if (LZ4F_isError(ret)) {
        return -3;
    }

    *written = dst_size;

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

    size_t read = fread(input, 1, sizeof(input), in);

    if (ferror(in)) {
        fclose(in);
        fclose(out);
        return -3;
    }

    size_t written;

    static uint8_t compressed[LZ4_COMPRESSBOUND(1024 * 1024) + 256];
    
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

    // Scriviamo DIRETTAMENTE il buffer compresso, senza aggiungere null'altro.
    // L'header standard è già dentro "compressed".
    fwrite(compressed, 1, written, out);

    fclose(in);
    fclose(out);

    return 0;
}
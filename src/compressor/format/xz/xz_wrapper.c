#include <internal/xz_wrapper.h>

#include <zstd.h>
#include <zlib.h>
#include <lz4.h>
#include <lzma.h>
#include <stdlib.h>

int compress_xz(
    const void *src,
    size_t src_size,
    void **dst,
    size_t *dst_size,
    int level)
{

    size_t out_cap = src_size + (src_size / 3) + 1024;

    void *out = malloc(out_cap);
    if (!out)
        return -1;

    size_t out_pos = 0;

    lzma_ret ret = lzma_easy_buffer_encode(
        level,
        LZMA_CHECK_CRC64,
        NULL,
        src,
        src_size,
        out,
        &out_pos,
        out_cap);

    if (ret != LZMA_OK) {
        free(out);
        return -2;
    }

    *dst = out;
    *dst_size = out_pos;

    return 0;
}

int decompress_xz(
    const void *src,
    size_t src_size,
    void **dst,
    size_t *dst_size,
    int level)
{
    size_t out_cap = src_size * 8;

    if (out_cap < 4096)
        out_cap = 4096;

    void *out = malloc(out_cap);
    if (!out)
        return -1;

    size_t in_pos = 0;
    size_t out_pos = 0;

    uint64_t memlimit = UINT64_MAX;

    lzma_ret ret;

    while (1) {

        ret = lzma_stream_buffer_decode(
            &memlimit,
            0,
            NULL,
            src,
            &in_pos,
            src_size,
            out,
            &out_pos,
            out_cap);

        if (ret == LZMA_OK)
            break;

        if (ret != LZMA_BUF_ERROR) {
            free(out);
            return -2;
        }

        out_cap *= 2;

        void *tmp = realloc(out, out_cap);
        if (!tmp) {
            free(out);
            return -3;
        }

        out = tmp;
    }

    *dst = out;
    *dst_size = out_pos;

    return 0;
}
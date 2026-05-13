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
    if (!src || !dst || !dst_size)
        return -1;

    z_stream zs;
    memset(&zs, 0, sizeof(zs));

    int rc = deflateInit2(
        &zs,
        level,
        Z_DEFLATED,
        15 + 16,   // gzip
        8,
        Z_DEFAULT_STRATEGY
    );

    if (rc != Z_OK)
        return -2;

    /*
     * margine extra per header/trailer gzip
     */
    uLong bound = compressBound(src_size) + 32;

    void *out = malloc(bound);
    if (!out)
    {
        deflateEnd(&zs);
        return -3;
    }

    zs.next_in = (Bytef *)src;
    zs.avail_in = (uInt)src_size;

    zs.next_out = (Bytef *)out;
    zs.avail_out = (uInt)bound;

    rc = deflate(&zs, Z_FINISH);

    if (rc != Z_STREAM_END)
    {
        deflateEnd(&zs);
        free(out);
        return -4;
    }

    *dst = out;
    *dst_size = zs.total_out;

    deflateEnd(&zs);

    return 0;
}

int gzip_decompress(
    const void *src,
    size_t src_size,
    void **dst,
    size_t *dst_size,
    int level)
{
    if (!src || !dst || !dst_size)
        return -1;

    /*
     * buffer iniziale
     */
    size_t out_cap = src_size * 4;

    if (out_cap < 1024)
        out_cap = 1024;

    void *out = malloc(out_cap);

    if (!out)
        return -2;

    z_stream zs;
    memset(&zs, 0, sizeof(zs));

    /*
     * 15 + 16 = gzip decoding
     */
    int rc = inflateInit2(&zs, 15 + 16);

    if (rc != Z_OK)
    {
        free(out);
        return -3;
    }

    zs.next_in = (Bytef *)src;
    zs.avail_in = (uInt)src_size;

    zs.next_out = (Bytef *)out;
    zs.avail_out = (uInt)out_cap;

    while (1)
    {
        rc = inflate(&zs, Z_NO_FLUSH);

        /*
         * decompressione completata
         */
        if (rc == Z_STREAM_END)
            break;

        /*
         * serve più output buffer
         */
        if (rc == Z_OK && zs.avail_out == 0)
        {
            out_cap *= 2;

            void *tmp = realloc(out, out_cap);

            if (!tmp)
            {
                inflateEnd(&zs);
                free(out);
                return -4;
            }

            out = tmp;

            zs.next_out =
                (Bytef *)out + zs.total_out;

            zs.avail_out =
                (uInt)(out_cap - zs.total_out);

            continue;
        }

        /*
         * gzip corrotto o incompleto
         */
        if (rc == Z_BUF_ERROR)
        {
            inflateEnd(&zs);
            free(out);
            return -5;
        }

        /*
         * errore zlib
         */
        inflateEnd(&zs);
        free(out);
        return -6;
    }

    *dst = out;
    *dst_size = zs.total_out;

    inflateEnd(&zs);

    return 0;
}
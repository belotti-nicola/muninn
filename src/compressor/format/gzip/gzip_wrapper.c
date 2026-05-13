#include <internal/gzip_wrapper.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
    size_t *dst_size)
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

int gzip_compress_file(const char *input_path)
{
    if (!input_path)
        return -1;

    FILE *f = fopen(input_path, "rb");
    if (!f)
        return -2;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (size <= 0)
    {
        fclose(f);
        return -3;
    }

    void *buffer = malloc(size);
    if (!buffer)
    {
        fclose(f);
        return -4;
    }

    size_t tmp_read = fread(buffer, 1, size, f);
    if (tmp_read != (size_t)size)
    {
        free(buffer);
        fclose(f);
        return -5;
    }
    fclose(f);

    void *compressed = NULL;
    size_t compressed_size = 0;

    int rc = gzip_compress(
        buffer,
        size,
        &compressed,
        &compressed_size,
        6
    );

    free(buffer);

    if (rc != 0)
        return -6;

    char out_path[1024];
    snprintf(out_path, sizeof(out_path), "%s.gz", input_path);

    FILE *out = fopen(out_path, "wb");
    if (!out)
    {
        free(compressed);
        return -7;
    }

    fwrite(compressed, 1, compressed_size, out);

    fclose(out);
    free(compressed);

    return 0;
}

int gzip_decompress_file(const char *input_path)
{
    if (!input_path)
        return -1;

    FILE *f = fopen(input_path, "rb");
    if (!f)
        return -2;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (size <= 0)
    {
        fclose(f);
        return -3;
    }

    void *compressed = malloc(size);
    if (!compressed)
    {
        fclose(f);
        return -4;
    }

    size_t tmp_read = fread(compressed, 1, size, f);
    if (tmp_read != (size_t)size)
    {
        free(compressed);
        fclose(f);
        return -5;
    }
    fclose(f);

    void *decompressed = NULL;
    size_t decompressed_size = 0;

    int rc = gzip_decompress(
        compressed,
        size,
        &decompressed,
        &decompressed_size
    );

    free(compressed);

    if (rc != 0)
        return -5;

    /*
     * output file: rimuove ".gz" se presente
     */
    char out_path[1024];

    const char *dot = strrchr(input_path, '.');

    if (dot && strcmp(dot, ".gz") == 0)
    {
        size_t len = dot - input_path;
        if (len >= sizeof(out_path))
            len = sizeof(out_path) - 1;

        snprintf(out_path, len + 1, "%s", input_path);
    }
    else
    {
        snprintf(out_path, sizeof(out_path), "%s.out", input_path);
    }

    FILE *out = fopen(out_path, "wb");
    if (!out)
    {
        free(decompressed);
        return -6;
    }

    fwrite(decompressed, 1, decompressed_size, out);

    fclose(out);
    free(decompressed);

    return 0;
}
#include <internal/munin_int.h>
#include <stdint.h>
#include <stdio.h>
#include <zstd.h>
#include <stdlib.h>
#include <time.h>

#include <internal/zstd_wrapper.h>

void create_unique_file_name(const char* base, char* out, size_t size)
{
    uint64_t now = (uint64_t)time(NULL);

    snprintf(
        out,
        size,
        "%s_%llu.zst",
        base,
        (unsigned long long)now
    );
}

int zstd_compress_file(const char* input_path)
{
    FILE* fin = fopen(input_path, "rb");
    if (!fin)
    {
        return 1;
    }
    
    int max_len = 64+P_SIZE+1;
    char output_path[max_len];
    create_unique_file_name(input_path,output_path,max_len);
    FILE* fout = fopen(output_path, "w");
    if (!fout)
    {
        return 1;
    }

    ZSTD_CCtx* cctx = ZSTD_createCCtx();

    size_t in_size = ZSTD_CStreamInSize();
    size_t out_size = ZSTD_CStreamOutSize();

    void* in_buf = malloc(in_size);
    void* out_buf = malloc(out_size);

    ZSTD_inBuffer input_buf = { in_buf, 0, 0 };
    ZSTD_outBuffer output_buf = { out_buf, out_size, 0 };

    int finished = 0;

    while (!finished) {

        input_buf.size = fread(in_buf, 1, in_size, fin);
        input_buf.pos = 0;

        finished = feof(fin);

        ZSTD_EndDirective mode =
            finished ? ZSTD_e_end : ZSTD_e_continue;

        while (input_buf.pos < input_buf.size || finished) {

            output_buf.pos = 0;

            size_t remaining = ZSTD_compressStream2(
                cctx,
                &output_buf,
                &input_buf,
                mode
            );

            if (ZSTD_isError(remaining)) {
                fprintf(stderr, "%s\n",
                        ZSTD_getErrorName(remaining));
                return 1;
            }

            fwrite(out_buf, 1, output_buf.pos, fout);

            if (finished && remaining == 0)
                break;
        }
    }

    ZSTD_freeCCtx(cctx);

    free(in_buf);
    free(out_buf);

    fclose(fin);
    fclose(fout);

    return 0;
}
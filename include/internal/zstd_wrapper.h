#ifndef ZSTD_WRAPPER_H
#define ZSTD_WRAPPER_H

#define BUF_SIZE 8192

int zstd_compress_file(const char* input_path);

#endif
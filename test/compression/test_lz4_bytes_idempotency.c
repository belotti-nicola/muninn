#include <internal/lz4_wrapper.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMP_MAX_SIZE 128

int main()
{
    const char bytes[] = "Hello world";
    int rc;

    char compressed[COMP_MAX_SIZE] = {0};
    size_t comp_actual_size;
    rc = lz4_compress(
        bytes,sizeof(bytes),
        &compressed,COMP_MAX_SIZE,&comp_actual_size,
        3); 
    if (rc != 0)
    {
        printf("Error: lz4 compression failed at line %d.",__LINE__);
        return 1;
    }

    char decompressed[COMP_MAX_SIZE] = {0};
    size_t decomp_actual_size;
    rc = lz4_decompress(
        compressed,comp_actual_size,
        &decompressed,COMP_MAX_SIZE,&decomp_actual_size);
    if (rc != 0)
    {
        printf("Error: lz4 decompression failed at line %d with code %d.",__LINE__,rc);
        return 1;
    }


    if (sizeof(bytes) != decomp_actual_size)
    {
        printf(
            "Error: size mismatch at line %d "
            "(original %zu, decompressed %zu)\n",
            __LINE__,
            sizeof(bytes),
            decomp_actual_size
        );

        return 1;
    }

    if(memcmp(bytes, decompressed, decomp_actual_size) != 0)
    {
        printf("Error: lz4 idempotency failed at line %d.",__LINE__);
        return 1;
    }
    
    
    return 0;
}
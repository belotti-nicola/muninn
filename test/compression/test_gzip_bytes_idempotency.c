#include <internal/gzip_wrapper.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    const char bytes[] = "Hello world";
    int rc;
    
    void *compressed = NULL;
    size_t compressed_size = 0;
    rc = gzip_compress(
        bytes,sizeof(bytes),
        &compressed,&compressed_size,
        3); 

    if (rc != 0)
    {
        printf("Error: gzip compression failed at line %d.",__LINE__);
        return 1;
    }

    void *decompressed = NULL;
    size_t decompressed_size;
    rc = gzip_decompress(
        compressed,compressed_size,
        &decompressed,&decompressed_size);
    if (rc != 0)
    {
        printf("Error: gzip decompression failed at line %d with code %d.",__LINE__,rc);
        return 1;
    }


    if (sizeof(bytes) != decompressed_size)
    {
        printf(
            "Error: size mismatch at line %d "
            "(original %zu, decompressed %zu)\n",
            __LINE__,
            sizeof(bytes),
            decompressed_size
        );

        return 1;
    }

    if(memcmp(bytes, decompressed, decompressed_size) != 0)
    {
        printf("Error: gzip idempotency failed at line %d.",__LINE__);
        return 1;
    }
    
    
    
    free(compressed);
    free(decompressed);
    return 0;
}
#include <stdint.h>
#include <stddef.h>

int format_uint8(  char *out,size_t out_size, uint8_t value );
int format_uint16( char *out,size_t out_size, uint16_t value );
int format_uint32( char *out,size_t out_size, uint32_t value );
int format_uint64( char *out,size_t out_size, uint64_t value );
#include <stdbool.h>
#include <stddef.h>

#include <stdbool.h>
#include <stdint.h>

bool n_format_uint8(  char *out,size_t out_size, size_t n, uint8_t value  );
bool n_format_uint16( char *out,size_t out_size, size_t n, uint16_t value );
bool n_format_uint32( char *out,size_t out_size, size_t n, uint32_t value );
bool n_format_uint64( char *out,size_t out_size, size_t n, uint64_t value );
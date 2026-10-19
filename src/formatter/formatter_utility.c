#include "formatter_utility.h"

#include <inttypes.h>
#include <stdio.h>

#define FORMAT_UINT8_MIN_SIZE    4U   /* "255" + '\0' */
#define FORMAT_UINT16_MIN_SIZE   6U   /* "65535" + '\0' */
#define FORMAT_UINT32_MIN_SIZE  11U   /* "4294967295" + '\0' */
#define FORMAT_UINT64_MIN_SIZE  21U   /* "18446744073709551615" + '\0' */

int format_uint8( char *out, size_t out_size, uint8_t value )
{
    if ( out == NULL ) return -1;
    if ( out_size < FORMAT_UINT8_MIN_SIZE ) return -2;

    return snprintf(out, out_size, "%03" PRIu8, value);
}

int format_uint16( char *out, size_t out_size, uint16_t value )
{
    if ( out == NULL ) return -1;
    if ( out_size < FORMAT_UINT16_MIN_SIZE ) return -2;

    return snprintf(out, out_size, "%05" PRIu16, value);
}

int format_uint32( char *out, size_t out_size, uint32_t value )
{
    if ( out == NULL ) return -1;
    if ( out_size < FORMAT_UINT32_MIN_SIZE ) return -2;

    return snprintf(out, out_size, "%010" PRIu32, value);
}

int format_uint64( char *out, size_t out_size, uint64_t value )
{
    if ( out == NULL ) return -1;
    if ( out_size < FORMAT_UINT64_MIN_SIZE ) return -2;

    return snprintf(out, out_size, "%010" PRIu64, value);
}
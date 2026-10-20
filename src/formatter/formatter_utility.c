#include "formatter_utility.h"

#include <inttypes.h>
#include <stdio.h>

#include <string.h>

bool n_format_uint8( char *out,size_t out_size, size_t n, uint8_t value)
{
    if ( out == NULL || n == 0 ) return false;

    if ( out_size < 3 ) return false;

    char temp[64];
    int len = snprintf(temp, sizeof(temp), "%" PRIu8, value);

    if ( n < len ) return false;

    char *ptr = out;
    size_t offset = n - len;

    if ( offset > 0 )
    {
        memset(ptr,' ',offset);
        ptr += offset;
    }

    strncpy(out,temp,len);

    return true;
}
bool n_format_uint16( char *out,size_t out_size, size_t n, uint16_t value )
{
    if ( out == NULL || n == 0 ) return false;
    
    if ( out_size < 5 ) return false;

    char temp[64];
    int len = snprintf(temp, sizeof(temp), "%" PRIu16, value);

    if ( n < len ) return false;

    char *ptr = out;
    size_t offset = n - len;

    if ( offset > 0 )
    {
        memset(ptr,' ',offset);
        ptr += offset;
    }

    strncpy(ptr,temp,len);

    return true;
}
bool n_format_uint32( char *out,size_t out_size, size_t n, uint32_t value )
{
    if ( out == NULL || n == 0 ) return false;
    
    if ( out_size < 10 ) return false;

    char temp[64];
    int len = snprintf(temp, sizeof(temp), "%" PRIu32, value);

    if ( n < len ) return false;

    char *ptr = out;
    size_t offset = n - len;

    if ( offset > 0 )
    {
        memset(ptr,' ',offset);
        ptr += offset;
    }

    strncpy(ptr,temp,len);

    return true;
    
}
bool n_format_uint64( char *out,size_t out_size, size_t n, uint64_t value )
{
    if ( out == NULL || n == 0 ) return false;
    
    if ( out_size < 20 ) return false;

    char temp[64] = {0};
    int len = snprintf(temp, sizeof(temp), "%" PRIu64, value);

    if ( n < len ) return false;

    char *ptr = out;
    size_t offset = n - len;

    if ( offset > 0 )
    {
        memset(ptr,' ',offset);
        ptr += offset;
    }

    strncpy(ptr,temp,len);

    return true;
}
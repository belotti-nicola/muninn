#include <internal/muninn_formatter.h>

#include <string.h>
#include "test_utils.h"

#define BUFFER_SIZE 1048
#define PATTERN "<msg>"

int main()
{
    char buffer[BUFFER_SIZE] = {0};

    char *buffer_msg = "Hello world...";
    size_t buffer_msg_size = strlen(buffer_msg);


    muninn_header  mh = {0};
    muninn_payload mp = {0};

    mp.msg     = buffer_msg;
    mp.msg_len = buffer_msg_size;

    muninn_message mm = {0};
    mm.header  = &mh;
    mm.payload = &mp;

    muninn_formatter_t formatter = {0};

    if ( muninn_formatter_init( &formatter,PATTERN,strlen(PATTERN) ) == false )
    {
        TEST_INFO("NOK muninn_formatter_init");
        return 1;
    }
    
    size_t buffer_size;
    if ( muninn_formatter_expand(&formatter,mm,buffer,&buffer_size, BUFFER_SIZE ) == false )
    {
        TEST_INFO("NOK muninn_formatter_expand");
        return 1;
    }

    if ( buffer_size != buffer_msg_size )
    {
        TRACE_ERROR_POSITION("Error: expanded bytes are %ld instead of %ld" , buffer_size , buffer_msg_size );
        return 1;
    }

    for ( int i = 0 ; i < buffer_msg_size ; i++ )
    {
        if ( buffer_msg[ i ] != buffer[ i ] )
        {
            TRACE_ERROR_POSITION("Error: found %u instead of %u" , buffer_msg[ i ] , buffer[ i ] );
            return 1;
        }
    }

    TEST_INFO("OK");
    return 0;
}
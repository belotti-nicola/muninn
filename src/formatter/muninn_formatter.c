#include "formatter_parser.h"

#include <internal/muninn_formatter.h>
#include <string.h>

bool muninn_formatter_init(muninn_formatter_t *mf, const char* pattern, size_t pattern_size)
{
    if ( mf == NULL || pattern == NULL || pattern_size == 0 ) return false;

    bool   retVal = parse_pattern_string(pattern,pattern_size,mf->tokens);
    return retVal;
}

bool muninn_formatter_expand(muninn_formatter_t *mf, muninn_message_t mm, char *buffer, size_t buffer_size)
{
    if ( mf == NULL || buffer == NULL || buffer_size == 0 ) return false;
   
    for ( int i = 0 ; i < MAX_TOKENS ; i++ )
    {
        muninn_formatter_token_t token = mf->tokens[i];
        switch(token.type)
        {
            default: break;
        }
    }

    memcpy(buffer, mm.msg, mm.msg_len);

    return true;
}
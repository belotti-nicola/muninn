#include "formatter_parser.h"

#include <internal/muninn_formatter.h>
#include <string.h>

bool muninn_formatter_init(muninn_formatter_t *mf, const char* pattern, size_t pattern_size)
{
    if ( mf == NULL || pattern == NULL || pattern_size == 0 ) return false;

    bool   retVal = parse_pattern_string(pattern,pattern_size,mf->tokens);
    return retVal;
}

bool muninn_formatter_expand(muninn_formatter_t *mf, muninn_message mm, char *out, size_t *out_size, size_t out_maximum_size)
{
    if ( mf == NULL || out == NULL || out_maximum_size == 0 || out_size == NULL ) return false;

    *out_size = 0;

    int bytes_counter = 0;
   
    for ( int i = 0 ; i < MAX_TOKENS ; i++ )
    {
        muninn_formatter_token_t token = mf->tokens[i];
        switch(token.type)
        {
            case M_TEXT:
            {
                memcpy(out,token.text,token.text_size);
                bytes_counter += token.text_size ;
                break;
            }
            case M_FILE:
            {
                memcpy(out,mm.payload->file,mm.payload->file_len);
                bytes_counter += mm.payload->file_len ;
                break;
            }
            case M_FUNCTION:
            {
                memcpy(out,mm.payload->func,mm.payload->func_len);
                bytes_counter += mm.payload->func_len ;
                break;
            }
            case M_LINE:
            {
                memcpy(out,&mm.payload->line,sizeof(uint32_t));
                bytes_counter += sizeof(uint32_t) ;
                break;
            }
            case M_MESSAGE:
            {
                memcpy(out,mm.payload->msg,mm.payload->msg_len);
                bytes_counter += mm.payload->msg_len ;
                break;
            }
            case M_PID:
            {
                memcpy(out,&mm.payload->pid,sizeof(mm.payload->pid));
                bytes_counter += sizeof( mm.payload->pid );
                break;
            }
            case M_SEVERITY:
            {
                memcpy(out,&mm.payload->severity,sizeof(mm.payload->severity));
                bytes_counter += sizeof( mm.payload->severity ); 
                break;
            }
            case M_THREAD_ID:
            {
                memcpy(out,&mm.payload->thread_id,sizeof(mm.payload->thread_id));
                bytes_counter += sizeof( mm.payload->thread_id );
                break;
            }
            case M_TIMESTAMP:
            {
                memcpy(out,&mm.payload->timestamp,sizeof(mm.payload->timestamp));
                bytes_counter += sizeof( mm.payload->timestamp );
                break;
            }
            default: 
            {
                break;
            }
        }
    }

    *out_size = bytes_counter ;

    return true;
}
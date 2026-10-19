#include "formatter_parser.h"

#include <internal/muninn_formatter.h>
#include "formatter_utility.h"
#include <string.h>

bool muninn_formatter_init(muninn_formatter_t *mf, const char* pattern, size_t pattern_size)
{
    if ( mf == NULL || pattern == NULL || pattern_size == 0 ) return false;

    size_t tokens_size;
    bool   retVal = parse_pattern_string(pattern,pattern_size,mf->tokens,&tokens_size);
    if ( retVal == false ) 
    {
        return false;
    }

    mf->tokens_size = tokens_size;

    return true;
}

bool muninn_formatter_expand(muninn_formatter_t *mf, muninn_message mm, char *out, size_t *out_size, size_t out_maximum_size)
{
    if ( mf == NULL || out == NULL || out_maximum_size == 0 || out_size == NULL ) return false;

    *out_size = 0;

    int bytes_counter = 0;
   
    for ( int i = 0 ; i < mf->tokens_size ; i++ )
    {
        muninn_formatter_token_t token = mf->tokens[i];
        switch(token.type)
        {
            case M_TEXT:
            {
                if ( bytes_counter + token.text_size > out_maximum_size )
                {
                    return false;
                }

                memcpy(out,token.text,token.text_size);
                bytes_counter += token.text_size ;
                out += bytes_counter;
                break;
            }
            case M_FILE:
            {
                if ( bytes_counter + mm.payload->file_len > out_maximum_size )
                {
                    return false;
                }

                memcpy(out,mm.payload->file,mm.payload->file_len);
                bytes_counter += mm.payload->file_len ;
                out += bytes_counter;
                break;
            }
            case M_FUNCTION:
            {
                if ( bytes_counter + mm.payload->func_len > out_maximum_size )
                {
                    return false;
                }
                
                memcpy(out,mm.payload->func,mm.payload->func_len);
                bytes_counter += mm.payload->func_len ;
                out += bytes_counter;
                break;
            }
            case M_LINE:
            {
                if ( bytes_counter + 10 > out_maximum_size )
                {
                    return false;
                }

                uint32_t tmp;
                memcpy(&tmp,&mm.payload->line,sizeof(mm.payload->line));
                size_t available_bytes = out_maximum_size - bytes_counter;
                if ( format_uint32(out,available_bytes,tmp) != 10 )
                {
                    return false;
                }
                bytes_counter += 10 ;
                out += bytes_counter;
                break;
            }
            case M_MESSAGE:
            {
                if ( bytes_counter + mm.payload->msg_len > out_maximum_size )
                {
                    return false;
                }

                memcpy(out,mm.payload->msg,mm.payload->msg_len);
                bytes_counter += mm.payload->msg_len ;
                out += bytes_counter;
                break;
            }
            case M_PID:
            {
                if ( bytes_counter + 10 > out_maximum_size )
                {
                    return false;
                }

                uint32_t tmp;
                memcpy(&tmp,&mm.payload->pid,sizeof(mm.payload->pid));
                size_t available_bytes = out_maximum_size - bytes_counter;
                if ( format_uint32(out,available_bytes,tmp) != 10 )
                {
                    return false;
                }
                bytes_counter += 10 ;
                out += bytes_counter;
                break;
            }
            case M_SEVERITY:
            {
                if ( bytes_counter + 3 > out_maximum_size )
                {
                    return false;
                }

                uint8_t tmp;
                memcpy(&tmp,&mm.payload->severity,sizeof(mm.payload->severity));
                size_t available_bytes = out_maximum_size - bytes_counter;
                if ( format_uint8(out,available_bytes,tmp) != 3 )
                {
                    return false;
                }
                bytes_counter += 3 ;
                out += bytes_counter;
                break;
            }
            case M_THREAD_ID:
            {
                if ( bytes_counter + 10 > out_maximum_size )
                {
                    return false;
                }
                
                uint64_t tmp;
                memcpy(&tmp,&mm.payload->thread_id,sizeof(mm.payload->thread_id));
                size_t available_bytes = out_maximum_size - bytes_counter;
                if ( format_uint64(out,available_bytes,tmp) != 10 )
                {
                    return false;
                }
                bytes_counter += 10 ;
                out += bytes_counter;
                break;
            }
            case M_TIMESTAMP:
            {
                if ( bytes_counter + 10 > out_maximum_size )
                {
                    return false;
                }

                uint64_t tmp;
                memcpy(&tmp,&mm.payload->timestamp,sizeof(mm.payload->timestamp));
                size_t available_bytes = out_maximum_size - bytes_counter;
                if ( format_uint64(out,available_bytes,tmp) != 10 )
                {
                    return false;
                }
                bytes_counter += 10 ;
                out += bytes_counter;
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
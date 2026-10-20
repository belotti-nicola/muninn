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

    char *backup = out;

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
                out += token.text_size;
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
                out += mm.payload->file_len;
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
                out += mm.payload->func_len;
                break;
            }
            case M_LINE:
            {
                size_t available_bytes = out_maximum_size - bytes_counter;
                if ( n_format_uint32(out,available_bytes,10,mm.payload->line) == false )
                {
                    return false;
                }
                bytes_counter += 10 ;
                out += 10;
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
                out += mm.payload->msg_len;
                break;
            }
            case M_PID:
            {
                size_t available_bytes = out_maximum_size - bytes_counter;
                if ( n_format_uint32(out,available_bytes,10,mm.payload->pid) == false )
                {
                    return false;
                }
                bytes_counter += 10 ;
                out += 10;
                break;
            }
            case M_SEVERITY:
            {
                size_t available_bytes = out_maximum_size - bytes_counter;
                if ( n_format_uint8(out,available_bytes,3,mm.payload->severity) == false )
                {
                    return false;
                }
                bytes_counter += 3 ;
                out += 3;
                break;
            }
            case M_THREAD_ID:
            {               
                size_t available_bytes = out_maximum_size - bytes_counter;
                if ( n_format_uint64(out,available_bytes,20,mm.payload->thread_id) == false )
                {
                    return false;
                }
                bytes_counter += 20 ;
                out += 20;
                break;
            }
            case M_TIMESTAMP:
            {
                size_t available_bytes = out_maximum_size - bytes_counter;
                if ( n_format_uint64(out,available_bytes,20,mm.payload->timestamp) == false )
                {
                    return false;
                }
                bytes_counter += 20 ;
                out += 20;
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
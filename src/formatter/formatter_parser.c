#include "formatter_parser.h"
#include "muninn_formatter.h"

bool parse_pattern_string(const char *string, size_t string_size, muninn_formatter_token_t *out)
{
    if ( string == NULL || string_size == 0 || out == NULL ) return false;

    int token_counter = 0;
    int state = 0;
    for ( int i = 0 ; i < string_size ; i++ )
    {
        char tmp = string[i];
        switch(state)
        {
            case 0: 
            {
                if ( tmp == '<' )
                {
                    if ( token_counter == MAX_TOKENS ) return false;

                    token_counter += 1;
                    state = 1;
                    break; 
                }

                state = 0;
                break;
            }
            case 1:
            {
                if ( tmp == 't' )
                {
                    state = 2;
                    break; 
                }
                if ( tmp == 'l' )
                {
                    state = 4;
                    break; 
                }
                if ( tmp == 's' )
                {
                    state = 7;
                    break; 
                }
                if ( tmp == 'p' )
                {
                    state = 9;
                    break; 
                }
                if ( tmp == 'f' )
                {
                    state = 11;
                    break; 
                }
                if ( tmp == 'm' )
                {
                    state = 13;
                    break; 
                }
            
                return false;
            }
            case 2:
            {
                if ( tmp == 's' )
                {
                    state = 3;
                    break; 
                }
                if ( tmp == 'i' )
                {
                    state = 15;
                    break; 
                }
                
                return false;
            }
            case 3:
            {
                if ( tmp != '>' )
                {
                    return false;
                }
                
                (out + token_counter)->type      = TIMESTAMP;
                (out + token_counter)->text[0]   = tmp;
                (out + token_counter)->text_size = 10;

                state = 0;
            }
            case 4:
            {
                if ( tmp == 'i' )
                {
                    state = 5;
                    break; 
                }
                
                return false;
            }
            case 5:
            {
                if ( tmp == 'n' )
                {
                    state = 6;
                    break; 
                }
                
                return false;
            }
            case 6:
            {
                if ( tmp == 'e' )
                {
                    state = 3;
                    break; 
                }
                
                return false;
            }
            case 7:
            {
                if ( tmp == 'e' )
                {
                    state = 8;
                    break; 
                }
                
                return false;
            }
            case 8:
            {
                if ( tmp == 'v' )
                {
                    state = 3;
                    break; 
                }
                
                return false;
            }
            case 9:
            {
                if ( tmp == 'i' )
                {
                    state = 10;
                    break; 
                }
                
                return false;
            }
            case 10:
            {
                if ( tmp == 'd' )
                {
                    state = 3;
                    break; 
                }
                
                return false;
            }
            case 11:
            {
                if ( tmp == 'u' )
                {
                    state = 12;
                    break; 
                }
                
                return false;
            }
            case 12:
            {
                if ( tmp == 'n' )
                {
                    state = 3;
                    break; 
                }
                
                return false;
            }
            case 13:
            {
                if ( tmp == 's' )
                {
                    state = 14;
                    break; 
                }
                
                return false;
            }
            case 14:
            {
                if ( tmp == 'g' )
                {
                    state = 3;
                    break; 
                }
                
                return false;
            }
            case 15:
            {
                if ( tmp == 'd' )
                {
                    state = 3;
                    break; 
                }
                
                return false;
            }
            default:
            {
                return false;
            }
        }
    }
}
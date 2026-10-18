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
                    state = 6;
                    break; 
                }
                if ( tmp == 's' )
                {
                    state = 10;
                    break; 
                }
                if ( tmp == 'p' )
                {
                    state = 13;
                    break; 
                }
                if ( tmp == 'f' )
                {
                    state = 16;
                    break; 
                }
                if ( tmp == 'm' )
                {
                    state = 19;
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
                    state = 4;
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

                (out + token_counter)->type = M_TIMESTAMP;
                token_counter++;
                
                state = 0;
                break;
            }
            case 4:
            {
                if ( tmp == '4' )
                {
                    state = 5;
                    break; 
                }
                
                return false;
            }
            case 5:
            {
                if ( tmp != '>' )
                {
                    return false; 
                }
                
                (out + token_counter)->type = M_THREAD_ID;
                state = 0;
                break;
            }
            case 6:
            {
                if ( tmp == 'i' )
                {
                    state = 7;
                    break; 
                }
                
                return false;
            }
            case 7:
            {
                if ( tmp == 'n' )
                {
                    state = 8;
                    break; 
                }
                
                return false;
            }
            case 8:
            {
                if ( tmp == 'e' )
                {
                    state = 9;
                    break; 
                }
                
                return false;
            }
            case 9:
            {
                if ( tmp != '>' )
                {
                    return false;
                }
                
                (out + token_counter)->type = M_LINE;
                state = 0;
                break;
            }
            case 10:
            {
                if ( tmp == 'e' )
                {
                    state = 11;
                    break; 
                }
                
                return false;
            }
            case 11:
            {
                if ( tmp == 'v' )
                {
                    state = 12;
                    break; 
                }
                
                return false;
            }
            case 12:
            {
                if ( tmp != '>' )
                {
                    return false;
                }
                
                (out + token_counter)->type = M_SEVERITY;
            }
            case 13:
            {
                if ( tmp == 'i' )
                {
                    state = 14;
                    break; 
                }
                
                return false;
            }
            case 14:
            {
                if ( tmp == 'd' )
                {
                    state = 15;
                    break; 
                }
                
                return false;
            }
            case 15:
            {
                if ( tmp != '>' )
                {
                    return false;
                }
                
                (out + token_counter)->type = M_PID;
                state = 0;
                break;
            }
            case 16:
            {
                if ( tmp == 'u' )
                {
                    state = 17;
                    break; 
                }
                
                return false;
            }
            case 17:
            {
                if ( tmp == 'n' )
                {
                    state = 18;
                    break; 
                }
                
                return false;
            }
            case 18:
            {
                if ( tmp != '>' )
                {
                    return false;
                }
                
                (out + token_counter)->type = M_FUNCTION;
                break;
            }
            case 19:
            {
                if ( tmp == 's' )
                {
                    state = 20;
                    break; 
                }
                
                return false;
            }
            case 20:
            {
                if ( tmp == 'g' )
                {
                    state = 21;
                    break; 
                }
                
                return false;
            }
            case 21:
            {
                if ( tmp != '>' )
                {
                    return false;
                }
                
                state = 0;
                (out + token_counter)->type = M_MESSAGE;
                break; 
            }
            default:
            {
                return false;
            }
        }
    }

    return true;
}
#ifndef MUNINN_FORMATTER_TOKENS_H
#define MUNINN_FORMATTER_TOKENS_H

#include <stddef.h>

#define TEXT_SIZE 64
 
typedef enum formatter_token_type_t
{
    M_TEXT,

    M_TIMESTAMP,
    M_THREAD_ID,
    M_LINE,
    M_SEVERITY,
    M_PID,
    M_FILE,
    M_FUNCTION, 
    M_MESSAGE
    
} formatter_token_type_t;

typedef struct muninn_formatter_token_t
{
    formatter_token_type_t type;
    char                   text[TEXT_SIZE];
    size_t                 text_size;

} muninn_formatter_token_t;

#endif
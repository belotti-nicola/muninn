#ifndef MUNINN_FORMATTER_TOKENS_H
#define MUNINN_FORMATTER_TOKENS_H

#include <stddef.h>

#define TEXT_SIZE 64
 
typedef enum formatter_token_type_t
{
    TEXT,

    TIMESTAMP,
    THREAD_ID,
    LINE,
    SEVERITY,
    PID,
    FILE,
    FUNCTION, 
    MESSAGE
    
} formatter_token_type_t;

typedef struct muninn_formatter_token_t
{
    formatter_token_type_t type;
    char                   text[TEXT_SIZE];
    size_t                 text_size;

} muninn_formatter_token_t;

#endif
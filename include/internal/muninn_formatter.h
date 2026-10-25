#ifndef MUNINN_FORMATTER_H
#define MUNINN_FORMATTER_H

#include <internal/protocols/muninn_encoded_messages/muninn_encoded_message.h>
#include <internal/muninn_formatter_tokens.h>
#include <stdbool.h>

#define MAX_TOKENS 50

typedef struct muninn_formatter_t 
{
    size_t tokens_size;
    muninn_formatter_token_t tokens[MAX_TOKENS];

} muninn_formatter_t;

bool muninn_formatter_init(muninn_formatter_t *mf, const char* pattern, size_t pattern_size);
bool muninn_formatter_expand(muninn_formatter_t *mf, muninn_message mm, char *out, size_t *out_size, size_t out_maximum_size);

#endif
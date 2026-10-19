#ifndef FORMATTER_PARSER_H
#define FORMATTER_PARSER_H

#include <stdbool.h>
#include <internal/muninn_formatter_tokens.h>

bool parse_pattern_string(const char *string, size_t string_size, muninn_formatter_token_t *out, size_t *tokens);

#endif
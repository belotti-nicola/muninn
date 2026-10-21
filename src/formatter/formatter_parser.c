#include "formatter_parser.h"
#include "muninn_formatter.h"
#include <string.h>

typedef struct 
{
    const char *name;
    formatter_token_type_t type;
} tag_mapping_t;

// TODO: WE CAN GENERATE THIS
static const tag_mapping_t TAG_TABLE[] = {
    {"ts",   M_TIMESTAMP},
    {"tid",  M_THREAD_ID},
    {"line", M_LINE},
    {"sev",  M_SEVERITY},
    {"pid",  M_PID},
    {"fun", M_FUNCTION},
    {"msg",  M_MESSAGE},
    {"file", M_FILE}
};

bool parse_pattern_string(const char *string, size_t string_size, muninn_formatter_token_t *out, size_t *tokens)
{
    if (string == NULL || string_size == 0 || out == NULL || tokens == NULL) return false;

    size_t i = 0;
    size_t token_counter = 0;
    size_t text_counter  = 0; // THIS CAN'T EXCEED TEXT_SIZE ( FORMATTER SIZE )

    while (i < string_size) 
    {
        if (token_counter >= MAX_TOKENS) return false;

        if (string[i] == '<') 
        {
            size_t start = ++i;
            
            while (i < string_size && string[i] != '>') 
            {
                i++;
            }

            if (i >= string_size) return false; // TAG IS NOT CLOSED

            size_t tag_len = i - start;
            
            bool found = false;
            formatter_token_type_t found_type;
            for (size_t t = 0; t < sizeof(TAG_TABLE) / sizeof(TAG_TABLE[0]); t++) 
            {
                if (tag_len == strlen(TAG_TABLE[t].name) && 
                    strncmp(&string[start], TAG_TABLE[t].name, tag_len) == 0) 
                {
                    found      = true;
                    found_type = TAG_TABLE[t].type;
                    break;
                }
            }

            if (found == false) return false; // TAG IS NOT RECOGNIZED

            out[token_counter].type = found_type;

            token_counter++;
            i++;
        } 
        else 
        {
            size_t text_start = i;
            while ( i < string_size && string[i] != '<' && ( i - text_start < TEXT_SIZE ) ) i++;

            size_t text_len = i - text_start;

            if (token_counter >= MAX_TOKENS) return false;
                
            strncpy(out[token_counter].text, &string[text_start], text_len);
            out[token_counter].text[text_len] = '\0';
            out[token_counter].text_size = text_len;
            out[token_counter].type = M_TEXT;
                            
            token_counter++;
            
        }
    }

    *tokens = token_counter;
    return true;
}

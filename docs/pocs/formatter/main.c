#include <stddef.h>

#include <stdio.h>
#include <string.h>

#define MAX_FORMAT_TOKENS 16

typedef enum {
    TOKEN_TEXT,       
    TOKEN_TIMESTAMP,  
    TOKEN_SEVERITY,
    TOKEN_MESSAGE  
} token_type_t;

typedef struct {
    token_type_t type;
    char text[64];    
    size_t len;

} format_token_t;

#define MAX_FORMAT_TOKENS 16
#define MAX_OUTPUT_LINE   128


void compile_log_pattern(const char *pattern, format_token_t *tokens, size_t *token_count) 
{
    size_t count = 0;
    const char *ptr = pattern;

    while (*ptr && count < MAX_FORMAT_TOKENS) 
    {
        if (*ptr == '%') 
        {
            ptr++; //READ WHAT FOLLOWS
            if (*ptr == 'T') 
            {
                tokens[count++].type = TOKEN_TIMESTAMP;
            } else if (*ptr == 'L') 
            {
                tokens[count++].type = TOKEN_SEVERITY;
            } else if (*ptr == 'm') 
            {
                tokens[count++].type = TOKEN_MESSAGE;
            }
            ptr++;
        } 
        else 
        {
            // keep reading chars
            tokens[count].type = TOKEN_TEXT;
            size_t i = 0;
            while (*ptr && *ptr != '%' && i < 63) 
            {
                tokens[count].text[i++] = *ptr++;
            }
            tokens[count].text[i] = '\0';
            tokens[count].len = i;
            count++;
        }
    }
    *token_count = count;
}

size_t format_log_line(char *dest_buffer, format_token_t *tokens, size_t token_count, 
                       const char *raw_msg, const char *severity, const char *timestamp) 
{
    size_t offset = 0;

    for (size_t i = 0; i < token_count; i++) 
    {
        switch (tokens[i].type) 
        {
            case TOKEN_TEXT:
                memcpy(dest_buffer + offset, tokens[i].text, tokens[i].len);
                offset += tokens[i].len;
                break;
                
            case TOKEN_TIMESTAMP:
                size_t ts_len = strlen(timestamp);
                memcpy(dest_buffer + offset, timestamp, ts_len);
                offset += ts_len;
                break;
                
            case TOKEN_SEVERITY:
                size_t sev_len = strlen(severity);
                memcpy(dest_buffer + offset, severity, sev_len);
                offset += sev_len;
                break;
                
            case TOKEN_MESSAGE:
                size_t msg_len = strlen(raw_msg);
                memcpy(dest_buffer + offset, raw_msg, msg_len);
                offset += msg_len;
                break;
        }
    }
}

int main()
{
    const char *style1 = "%T|%L|%m -> 1 style";
    format_token_t tokens1[MAX_FORMAT_TOKENS];
    size_t token_count1;
    compile_log_pattern(style1,tokens1,&token_count1);

    const char *style2 = "%L|%T|%m -> 2 style";
    format_token_t tokens2[MAX_FORMAT_TOKENS];
    size_t token_count2;
    compile_log_pattern(style2,tokens2,&token_count2);


    char out[MAX_OUTPUT_LINE] = {0};
    const char *message   = "Hello world";
    const char *timestamp = "1182816000";
    const char *severity  = "1";
    
    format_log_line(out,
        tokens1,token_count1,
        message,severity,timestamp);
    printf("%s\n",out);

    out[0] = '\0';

    format_log_line(out,
        tokens2,token_count2,
        message,severity,timestamp);
    printf("%s\n",out);

    return 0;
}
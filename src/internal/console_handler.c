#include <internal/console_handler.h>
#include <unistd.h>
#include <stdio.h>

#include <internal/muninn_int.h>

const char * const MUNINN_COLOR_STRINGS[__COLOR_MAX__] = {
    [COLOR_BLACK]   = "\x1b[30m",
    [COLOR_RED]     = "\x1b[31m",
    [COLOR_GREEN]   = "\x1b[32m",
    [COLOR_YELLOW]  = "\x1b[33m",
    [COLOR_BLUE]    = "\x1b[34m",
    [COLOR_MAGENTA] = "\x1b[35m",
    [COLOR_CYAN]    = "\x1b[36m",
    [COLOR_WHITE]   = "\x1b[37m",
    [COLOR_RESET]   = "\x1b[0m"
};

void console_handler_setup(console_handler_t *oh)
{
    if (oh == NULL) return;
    
    bool is_terminal = isatty(fileno(stdout));
    oh->is_terminal = is_terminal;
}

void console_handler(console_handler_t *oh, uint8_t severity, const char *msg, size_t msg_len)
{
    if (oh == NULL || msg == NULL) return;

    MUNINN_COLORS color_idx = COLOR_RESET;
    switch (severity) 
    {
        case LOG_NONE:  color_idx = COLOR_WHITE;   break;
        case LOG_DEBUG: color_idx = COLOR_CYAN;    break;
        case LOG_INFO:  color_idx = COLOR_GREEN;   break;
        case LOG_WARN:  color_idx = COLOR_YELLOW;  break;
        case LOG_ERROR: color_idx = COLOR_RED;     break;
        case LOG_FATAL: color_idx = COLOR_MAGENTA; break;
    }

    if (oh->is_terminal && oh->is_color) 
    {
        const char *color = MUNINN_COLOR_STRINGS[color_idx];
        const char *reset = MUNINN_COLOR_STRINGS[COLOR_RESET];

        fprintf(stdout, "%s%.*s%s\n", 
                color, 
                (int)msg_len, 
                msg, 
                reset
        );
    } 
    else 
    {
        fprintf(stdout, "%.*s\n", (int)msg_len, msg);
    }
}
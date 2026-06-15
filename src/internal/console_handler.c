#include <internal/console_handler.h>
#include <unistd.h>
#include <stdio.h>

#include <internal/munin_int.h>

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

    const char *sev_str = "UNKN ";
    MUNINN_COLORS color_idx = COLOR_RESET;
    switch (severity) {
        case LOG_NONE:  sev_str = "NONE "; color_idx = COLOR_WHITE;   break;
        case LOG_DEBUG: sev_str = "DEBUG"; color_idx = COLOR_CYAN;    break;
        case LOG_INFO:  sev_str = "INFO "; color_idx = COLOR_GREEN;   break;
        case LOG_WARN:  sev_str = "WARN "; color_idx = COLOR_YELLOW;  break;
        case LOG_ERROR: sev_str = "ERROR"; color_idx = COLOR_RED;     break;
        case LOG_FATAL: sev_str = "FATAL"; color_idx = COLOR_MAGENTA; break;
    }

    if (oh->is_terminal) 
    {
        fprintf(stdout, "%s[%s] %.*s%s\n", 
                MUNINN_COLOR_STRINGS[color_idx], 
                sev_str, 
                (int)msg_len, 
                msg, 
                MUNINN_COLOR_STRINGS[COLOR_RESET]);
    } 
    else 
    {
        fprintf(stdout, "[%s] %.*s\n", sev_str, (int)msg_len, msg);
    }
}
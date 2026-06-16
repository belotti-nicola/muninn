#ifndef OUT_HANDLER_H
#define OUT_HANDLER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <internal/log_types.h>
#include <internal/muninn_int.h>

// you can not modify nor pointers nor values 
extern const char * const MUNINN_COLOR_STRINGS[__COLOR_MAX__];

typedef struct console_handler_s
{
    bool is_terminal;

} console_handler_t;

void console_handler_setup(console_handler_t *oh);

void console_handler(console_handler_t *oh, uint8_t severity, const char *msg, size_t msg_len);

#endif
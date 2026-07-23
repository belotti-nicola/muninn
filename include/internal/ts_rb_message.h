#ifndef TS_RB_MESSAGE_H
#define TS_RB_MESSAGE_H

#include <stddef.h>
#include <stdint.h>


typedef struct ts_rb_message_t 
{
    uint8_t  *buffer;
    uint16_t  buffer_size;

    uint16_t  max_size;

} ts_rb_message_t;

void ts_rb_message_setup(
    ts_rb_message_t *tsrb_message,
    uint8_t         *buffer,
    size_t           buffer_size
);

void ts_rb_message_set(
    ts_rb_message_t *tsrb_message,
    uint8_t         *in,
    size_t           in_size
);


#endif
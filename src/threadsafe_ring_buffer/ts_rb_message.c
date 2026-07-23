#include <internal/ts_rb_message.h>
#include <string.h>

void ts_rb_message_setup(
    ts_rb_message_t *tsrb_message,
    uint8_t  *buffer,
    size_t    buffer_size)
{
    if(tsrb_message == NULL || buffer == NULL || buffer_size == 0) return;

    tsrb_message->buffer = buffer;
    tsrb_message->buffer_size = 0;
    tsrb_message->max_size     = buffer_size;
}

void ts_rb_message_set(
    ts_rb_message_t *tsrb_message,
    uint8_t *in,
    size_t   in_size)
{
    if(tsrb_message == NULL || in == NULL || in_size == 0) return;

    if(tsrb_message->buffer == NULL || tsrb_message->max_size == 0) return;

    if(tsrb_message->max_size < in_size) return;

    memcpy(tsrb_message->buffer,in,in_size);
    tsrb_message->buffer_size = in_size;
}


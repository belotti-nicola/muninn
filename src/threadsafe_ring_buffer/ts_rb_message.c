#include <internal/ts_rb_message.h>

void ts_rb_message_setup(ts_rb_message_t *tsrb_message,
    uint8_t  *payload_bytes,
    size_t    payload_bytes_max_size)
{
    tsrb_message->header.msg_len  = 0;
    tsrb_message->header.ts       = 0;
    tsrb_message->header.severity = 0;

    tsrb_message->payload.payload_bytes          = payload_bytes;
    tsrb_message->payload.payload_bytes_max_size = payload_bytes_max_size;
    
}

void ts_rb_message_set(ts_rb_message_t *tsrb_message,
    uint32_t  msg_len,
    uint64_t  ts,
    uint8_t   severity,
    uint8_t  *payload_bytes)
{
    tsrb_message->header.msg_len  = msg_len;
    tsrb_message->header.ts       = ts;
    tsrb_message->header.severity = severity;

    tsrb_message->payload.payload_bytes  = payload_bytes;
}




#ifndef TS_RB_MESSAGE_H
#define TS_RB_MESSAGE_H

#include <stddef.h>
#include <stdint.h>

typedef struct __attribute__((packed)) ts_rb_header_t 
{
    uint32_t msg_len;
    uint64_t ts;
    uint8_t  severity;

} ts_rb_header_t;

typedef struct ts_rb_payload_t
{
    uint8_t *payload_bytes;
    size_t   payload_bytes_max_size;

} ts_rb_payload_t;


typedef struct ts_rb_message_t 
{
    ts_rb_header_t  header;
    ts_rb_payload_t payload;

} ts_rb_message_t;

void ts_rb_message_setup(ts_rb_message_t *tsrb_message,
    uint8_t  *payload_bytes,
    size_t    payload_bytes_max_size
);

void ts_rb_message_set(ts_rb_message_t *tsrb_message,
    uint32_t  msg_len,
    uint64_t  ts,
    uint8_t   severity,
    uint8_t  *payload_bytes
);


#endif
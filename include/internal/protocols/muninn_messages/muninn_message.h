#ifndef MUNINN_MESSAGE_H
#define MUNINN_MESSAGE_H

#include <stddef.h>
#include <stdint.h>

#include <internal/protocols/muninn_messages/muninn_message_header.h>
#include <internal/protocols/muninn_messages/muninn_message_payload.h>

typedef struct muninn_message
{
    muninn_header  *header;
    muninn_payload *payload;
    
} muninn_message;

void muninn_message_set(muninn_message *message, muninn_header *header, muninn_payload *payload);


#endif
#ifndef MUNINN_MESSAGES_H
#define MUNINN_MESSAGES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum   muninn_message_mask muninn_message_mask;
typedef struct muninn_message      muninn_message;

bool muninn_messages_encode(muninn_message *mm, muninn_message_mask mmm, uint8_t *out, size_t *out_size);
bool muninn_messages_decode(const uint8_t *in, size_t in_size, muninn_message *mm);


#endif
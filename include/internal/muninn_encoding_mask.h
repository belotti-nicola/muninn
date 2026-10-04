#ifndef MUNINN_ENCODING_MASK
#define MUNINN_ENCODING_MASK

#include <internal/protocols/muninn_messages/muninn_message_mask.h>
#include <stdint.h>
#include <stdatomic.h>

typedef struct muninn_encoding_mask_t 
{
    _Atomic uint32_t mask;
    
} muninn_encoding_mask_t;

void                muninn_encoding_mask_init(muninn_encoding_mask_t *muninn_encoding_mask, muninn_message_mask initial_value);
muninn_message_mask muninn_encoding_mask_get(muninn_encoding_mask_t *muninn_encoding_mask);
void                muninn_encoding_mask_set(muninn_encoding_mask_t *muninn_encoding_mask, muninn_message_mask mask);


#endif
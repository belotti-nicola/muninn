#include <internal/muninn_encoding_mask.h>
#include <stdio.h>

void muninn_encoding_mask_init(muninn_encoding_mask_t *muninn_encoding_mask, muninn_message_mask initial_value)
{
    atomic_init(&muninn_encoding_mask->mask, initial_value);
}

muninn_message_mask muninn_encoding_mask_get(muninn_encoding_mask_t *muninn_encoding_mask)
{
    if ( muninn_encoding_mask == NULL )
    {
        return MEDM_NONE;
    }

    return atomic_load(&muninn_encoding_mask->mask);

}
void muninn_encoding_mask_set(muninn_encoding_mask_t *muninn_encoding_mask, muninn_message_mask mask)
{
    if ( muninn_encoding_mask == NULL )
    {
        return;
    }

    atomic_store(
        &muninn_encoding_mask->mask,
        mask
    );
}
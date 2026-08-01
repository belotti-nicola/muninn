#include <internal/protocols/muninn_messages/muninn_message.h>

void muninn_message_set(muninn_message *mm, muninn_header *mh, muninn_payload *mp)
{
    mm->header  = mh;
    mm->payload = mp;
}

void muninn_message_set_by_payload(muninn_message *mm, muninn_payload *mp)
{
    
}


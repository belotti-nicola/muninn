#include <internal/protocols/messages/messages_codec.h>

#include "test_utils.h"

#define BUFFER_SIZE 256

int main()
{
    mnn_data_t mnn;
    const ENCONDED_DATA_MASK mask = MEDM_ALL;

    const uint8_t buffer[BUFFER_SIZE] = {0};
    size_t decoded_bytes;
    
    mm_decode(&mnn,buffer,BUFFER_SIZE,&decoded_bytes);
   
}
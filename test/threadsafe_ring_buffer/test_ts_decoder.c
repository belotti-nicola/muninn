#include <internal/ts_rb_decoder.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>

#define DECODED_BYTES_NUMBER 100


int main()
{
    uint8_t decoder_buffer[DECODED_BYTES_NUMBER] = {0};
    decoder_buffer[0]  = UINT8_C(0x0A);

    decoder_buffer[1]  = UINT8_C(0x78);
    decoder_buffer[2]  = UINT8_C(0x90);
    decoder_buffer[3]  = UINT8_C(0x4A);
    decoder_buffer[4]  = UINT8_C(0x2C);

    decoder_buffer[5]  = UINT8_C(0x63);  
    decoder_buffer[6]  = UINT8_C(0x7B);
    decoder_buffer[7]  = UINT8_C(0xDE);
    decoder_buffer[8]  = UINT8_C(0xC8);
    decoder_buffer[9]  = UINT8_C(0x4B);
    decoder_buffer[10] = UINT8_C(0x59);
    decoder_buffer[11] = UINT8_C(0x0C);
    decoder_buffer[12] = UINT8_C(0x17);

    decoder_buffer[13] = UINT8_C(0x12);
    decoder_buffer[14] = UINT8_C(0x2F); 

    decoder_buffer[15] = UINT8_C(0x2A);
    decoder_buffer[16] = UINT8_C(0x4D);
    decoder_buffer[17] = UINT8_C(0x4A);
    decoder_buffer[18] = UINT8_C(0xC7);
    decoder_buffer[19] = UINT8_C(0xAE);

    uint8_t first;
    uint32_t second;
    uint64_t third;
    uint16_t fourth;
    uint8_t fifth[5];

    bool rc;
    decoder_t decoder;
    decoder_setup(&decoder,decoder_buffer,20);
    rc = decode_u8(&decoder,&first);
    if(rc == false)
    {
        printf("Error at line %d: decode_u8 error!\n",__LINE__);
        return 1;
    }
    if(first != 10)
    {
        printf("Error at line %d: computed value is %d instead of %d!\n",__LINE__,first,10);
        return 1;
    }
    rc = decode_u32(&decoder,&second);
    if(rc == false)
    {
        printf("Error at line %d: decode_u8 error!\n",__LINE__);
        return 1;
    }
    if(second != UINT32_C(0x2C4A9078))
    {
        printf("Error at line %d: computed value is  %" PRIu32 " instead of %" PRIu32 "!\n",__LINE__,second,UINT32_C(0x78904a2c));
        return 1;
    }
    rc = decode_u64(&decoder,&third);
    if(rc == false)
    {
        printf("Error at line %d: decode_u8 error!\n",__LINE__);
        return 1;
    }
    if(third != UINT64_C(0x170C594BC8DE7B63))
    {
        printf("Error at line %d: computed value is %ld instead of %ld!\n",__LINE__,third,7168568183729228823);
        return 1;
    }
    rc = decode_u16(&decoder,&fourth);
    if(rc == false)
    {
        printf("Error at line %d: decode_u8 error!\n",__LINE__);
        return 1;
    }
    if(fourth !=  UINT64_C(0x2F12))
    {
        printf("Error at line %d: computed value is %d instead of %d!\n",__LINE__,fourth,4655);
        return 1;
    }
    rc = decode_bytes(&decoder,fifth,5);
    if(rc == false)
    {
        printf("Error at line %d: decode_u8 error!\n",__LINE__);
        return 1;
    }
    if(strncmp(decoder_buffer,decoder.buffer,5) != 0)
    {
        printf("Error at line %d: strncmp fail.\n",__LINE__);
        return 1;
    }

    return 0;
}
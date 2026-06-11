#include <internal/ts_rb_encoder.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


#define ENCODED_BYTES_NUMBER 100


int main()
{
    uint8_t encoded_bytes[ENCODED_BYTES_NUMBER] = {0};
    encoder_t encoder = 
    {
        .buffer = encoded_bytes,
        .buffer_size = ENCODED_BYTES_NUMBER,
        .offset = 0
    };

    bool b;

    b = encode_u8(&encoder,1);
    if(b == false)
    {
        printf("Error at line %d: encode fail.\n",__LINE__);
        return 1;
    }
    if(encoder.offset != sizeof(uint8_t))
    {
        printf("Error at line %d: offset found to be %ld instead of expected %ld\n",__LINE__,encoder.offset,sizeof(uint8_t));
        return 1;
    }
    if(encoder.buffer[0] != 1)
    {
        printf("Error at line %d: buffer at index %i found to be %d instead of %d\n",__LINE__,0,encoder.buffer[0],1);
        return 1;
    }

    b = encode_u32(&encoder,UINT32_C(0x79528CC1));
    if(b == false)
    {
        printf("Error at line %d: encode fail.\n",__LINE__);
        return 1;
    }
    if(encoder.offset != sizeof(uint32_t) + sizeof(uint8_t))
    {
        printf("Error at line %d: offset found to be %ld instead of %ld\n",__LINE__,encoder.offset,sizeof(uint8_t));
        return 1;
    }
    if(encoder.buffer[1] != 193)
    {
        printf("Error at line %d: found %d instead of %d\n",__LINE__,encoder.buffer[1],193);
        return 1;
    }
    if(encoder.buffer[2] != 140)
    {
        printf("Error at line %d: found %d instead of %d\n",__LINE__,encoder.buffer[2],140);
        return 1;
    }
    if(encoder.buffer[3] != 82)
    {
        printf("Error at line %d: found %d instead of %d\n",__LINE__,encoder.buffer[3],82);
        return 1;
    }
    if(encoder.buffer[4] != 121)
    {
        printf("Error at line %d: found %d instead of %d\n",__LINE__,encoder.buffer[4],121);
        return 1;
    }


    b = encode_u64(&encoder,UINT64_C(0xD59997ED67E33653));
    if(b == false)
    {
        printf("Error at line %d: encode fail.\n",__LINE__);
        return 1;
    }
    if(encoder.offset != sizeof(uint32_t) + sizeof(uint8_t) + sizeof(uint64_t))
    {
        printf("Error at line %d: offset found to be %ld instead of %ld\n",__LINE__,encoder.offset,
                    sizeof(uint32_t) + sizeof(uint8_t) + sizeof(uint64_t)
        );
        return 1;
    }
    if(encoder.buffer[5] != 83)
    {
        printf("Error at line %d: found %d instead of %d\n",__LINE__,encoder.buffer[5],83);
        return 1;
    }
    if(encoder.buffer[6] != 54)
    {
        printf("Error at line %d: found %d instead of %d\n",__LINE__,encoder.buffer[6],54);
        return 1;
    }
    if(encoder.buffer[7] != 227)
    {
        printf("Error at line %d: found %d instead of %d\n",__LINE__,encoder.buffer[7],227);
        return 1;
    }
    if(encoder.buffer[8] != 103)
    {
        printf("Error at line %d: found %d instead of %d\n",__LINE__,encoder.buffer[8],103);
        return 1;
    }
    if(encoder.buffer[9] != 237)
    {
        printf("Error at line %d: found %d instead of %d\n",__LINE__,encoder.buffer[9],237);
        return 1;
    }
    if(encoder.buffer[10] != 151)
    {
        printf("Error at line %d: found %d instead of %d\n",__LINE__,encoder.buffer[10],151);
        return 1;
    }
    if(encoder.buffer[11] != 153)
    {
        printf("Error at line %d: found %d instead of %d\n",__LINE__,encoder.buffer[11],153);
        return 1;
    }
    if(encoder.buffer[12] != 213)
    {
        printf("Error at line %d: found %d instead of %d\n",__LINE__,encoder.buffer[12],213);
        return 1;
    }

    char *tmp = "Hello World";
    b = encode_bytes(&encoder,tmp,strlen(tmp));
    if(b == false)
    {
        printf("Error at line %d: encode fail.\n",__LINE__);
        return 1;
    }
    if(encoder.offset != sizeof(uint32_t) + sizeof(uint8_t) + sizeof(uint64_t) + strlen(tmp))
    {
        printf("Error at line %d: offset found to be %ld instead of %ld\n",__LINE__,encoder.offset,
                    sizeof(uint32_t) + sizeof(uint8_t) + sizeof(uint64_t) + strlen(tmp)
        );
        return 1;
    }
    if(strncmp(tmp,encoder.buffer + 13,strlen(tmp)) != 0)
    {
        printf("Error at line %d: strcmp failed!\n",__LINE__);
        return 1;
    }

    b = encode_u16(&encoder,UINT16_C(0xA164));
    if(b == false)
    {
        printf("Error at line %d: encode fail.\n",__LINE__);
        return 1;
    }
    if(encoder.offset != sizeof(uint32_t) + sizeof(uint8_t) + sizeof(uint64_t) + strlen(tmp) + sizeof(uint16_t))
    {
        printf("Error at line %d: offset found to be %ld instead of %ld\n",__LINE__,
            encoder.offset,
            sizeof(uint32_t) + sizeof(uint8_t) + sizeof(uint64_t) + strlen(tmp) + sizeof(uint16_t));
        return 1;
    }
    if(encoder.buffer[24] != 100)
    {
        printf("Error at line %d: found %d instead of %d\n",__LINE__,encoder.buffer[24],100);
        return 1;
    }
    if(encoder.buffer[25] != 161)
    {
        printf("Error at line %d: found %d instead of %d\n",__LINE__,encoder.buffer[25],161);
        return 1;
    }



    return 0;
}
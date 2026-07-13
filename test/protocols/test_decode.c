#include <internal/protocols/messages/messages_codec.h>
#include <internal/protocols/messages/encoded_data_mask.h>
#include "reader/csv_reader.h"

#include <string.h>
#include <inttypes.h>

#include "test_utils.h"

#define BUFFER_SIZE 256


#define U8(x) ((uint8_t)(x))

#define LE16(x) \
    U8((x) & 0xFF), \
    U8(((x) >> 8) & 0xFF)

#define LE32(x) \
    U8((x) & 0xFF), \
    U8(((x) >> 8) & 0xFF), \
    U8(((x) >> 16) & 0xFF), \
    U8(((x) >> 24) & 0xFF)

#define LE64(x) \
    U8(((uint64_t)(x)) & 0xFF), \
    U8((((uint64_t)(x)) >>  8) & 0xFF), \
    U8((((uint64_t)(x)) >> 16) & 0xFF), \
    U8((((uint64_t)(x)) >> 24) & 0xFF), \
    U8((((uint64_t)(x)) >> 32) & 0xFF), \
    U8((((uint64_t)(x)) >> 40) & 0xFF), \
    U8((((uint64_t)(x)) >> 48) & 0xFF), \
    U8((((uint64_t)(x)) >> 56) & 0xFF)

#define STR6(s) \
    (s)[0], (s)[1], (s)[2], (s)[3], (s)[4], (s)[5]
#define STR3(s) \
    (s)[0], (s)[1], (s)[2]
#define STR11(s) \
    (s)[0], (s)[1], (s)[2], (s)[3], (s)[4], (s)[5],(s)[6], (s)[7], (s)[8], (s)[9], (s)[10]

int main()
{
    CSVReader reader;
    bool rc = csvreader_open(&reader,"data/codec_dataset.csv");
    if(rc == false)
    {   
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error");
        return 1;
    }

    uint8_t buf[4096] = {0};
    
    int iteration = 0;
    while(csvreader_next(&reader))
    {
        char *edm_st = reader.records[0];
        ENCODED_DATA_MASK mask = string_to_edm(edm_st);

        size_t records = reader.records_size;
        size_t offset  =  1 +
            (mask & MEDM_FILE? 1 : 0) +  
            (mask & MEDM_THREAD? 1 : 0) +  
            (mask & MEDM_TIMESTAMP? 1 : 0) +  
            (mask & MEDM_SEVERITY? 1 : 0) +  
            (mask & MEDM_MESSAGE? 1 : 0) +  
            (mask & MEDM_LINE? 1 : 0)+  
            (mask & MEDM_FUNCTION? 1 : 0)+  
            (mask & MEDM_PID? 1 : 0);

        size_t binary_len = 0; 
        for(size_t i = offset; i < reader.records_size; i++) 
        { 
            if (binary_len >= sizeof(buf)) 
            {
                TRACE_ERROR_POSITION();
                TEST_ERROR("Buffer di test troppo piccolo per l'estrazione dei byte!");
                csvreader_close(&reader);
                return 1;
            }

            buf[binary_len++] = (uint8_t)strtoul(reader.records[i], NULL, 0); 
        }

        mnn_data_t mnn = {0};
        uint8_t mnn_file_buf[256] = {0};
        uint8_t mnn_func_buf[256] = {0};
        uint8_t mnn_msg_buf[2048] = {0};
        mnn.file = mnn_file_buf;
        mnn.func = mnn_func_buf;
        mnn.msg  = mnn_msg_buf;
        bool rc = mm_decode(&mnn,buf,reader.records_size);
        if(rc == false)
        {
            TRACE_ERROR_POSITION();
            TEST_ERROR("mm_decode fail for iteration %d",iteration);
            return 1;
        }

        
        if( (mask & MEDM_SEVERITY) && reader.records_size >= 5)
        {
            int expected = STRING_TO_INT(reader.records[5]);
            if(expected != mnn.severity)
            {
                TRACE_ERROR_POSITION();
                TEST_ERROR("Iteration %d",iteration);
                TEST_ERROR("Computed %d but %d was expected",mnn.severity,expected);
                TEST_ERROR("(record %s)",reader.records[5]);
                return 1;
            }
        }
        

        iteration++;
    }
    
    return 0;
}
#include <internal/protocols/messages/messages_codec.h>

#include "test_utils.h"
#include "reader/csv_reader.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#define TEST_BUFFER_SIZE 2048

int main()
{
    uint8_t buffer[TEST_BUFFER_SIZE];
    size_t encoded_bytes = 0;
      
    CSVReader reader;
    bool rc = csvreader_open(&reader,"data/codec_dataset.csv");
    if(rc == false)
    {   
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error");
        return 1;
    }

    int record = 0;
    while(csvreader_next(&reader))
    {
        char **parsed = reader.records;
        mnn_data_t mnn2 = 
        {       
            .pid         = (uint32_t)strtoul(parsed[1], NULL, 10),
            .thread_id   = (uint64_t)strtoul(parsed[2], NULL, 10),
            .timestamp   = (uint64_t)strtoul(parsed[3], NULL, 10),
            .line        = (uint32_t)strtoul(parsed[4], NULL, 10),
            .severity    = (uint8_t)strtoul(parsed[5], NULL, 10),
            
            .file_len    = (uint8_t)strlen(parsed[6]),
            .file        = parsed[6],

            .func_len    = (uint8_t)strlen(parsed[7]),
            .func        = parsed[7],
            
            .msg_len     = (uint16_t)strlen(parsed[8]),
            .msg         = parsed[8],
        };

        //todo
        //ENCONDED_DATA_MASK mask = strtoul(parsed[0], NULL, 10);
        ENCONDED_DATA_MASK mask = MEDM_ALL;

        mm_encode(
            &mask,
            &mnn2,
            buffer,TEST_BUFFER_SIZE,
            &encoded_bytes
        );

        int expected_bytes = 29 + 4 +
            (int)strlen(parsed[6])+
            (int)strlen(parsed[7])+
            (int)strlen(parsed[8]);
        
        if(encoded_bytes != expected_bytes)
        {
            TRACE_ERROR_POSITION();
            TEST_ERROR("Encoded bytes %ld differs from %d for test case number %d",encoded_bytes,expected_bytes,record);
            return 1;
        }

        for(size_t tmp = 0 ; tmp < encoded_bytes ; tmp ++)
        {
            char *target = reader.records[tmp + 9];

            int expected_value = STRING_TO_INT(target);
            int computed_value = buffer[tmp];
            
            if( computed_value != expected_value)
            {
                TRACE_ERROR_POSITION();
                TEST_ERROR("Encoded byte differs from expected (tmp: %ld, record %d) record",tmp,record);
                TEST_ERROR("(computed %d, expected %d)",computed_value,expected_value);
                return 1;
            }

            TEST_INFO("Ok computed %d, expected %d)",computed_value,expected_value);
        }

        record++;
    }
    
    csvreader_close(&reader);

    return 0;
}
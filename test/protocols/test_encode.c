#include <internal/protocols/messages/messages_codec.h>

#include "test_utils.h"
#include "reader/csv_reader.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TEST_BUFFER_SIZE 256

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

        int test_check = 53;
        if(encoded_bytes != test_check)
        {
            TRACE_ERROR_POSITION();
            TEST_ERROR("Encoded bytes %ld differs from %d",encoded_bytes,test_check);
            return 1;
        }
    }
    
    csvreader_close(&reader);

    return 0;
}
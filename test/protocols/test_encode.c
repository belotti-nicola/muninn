#include <internal/protocols/messages/messages_codec.h>
#include <internal/protocols/messages/encoded_data_mask.h>


#include "test_utils.h"
#include "reader/csv_reader.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#define TEST_BUFFER_SIZE 4096

int main()
{
    uint8_t buffer[TEST_BUFFER_SIZE];
      
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
        size_t encoded_bytes = 0;

        char **parsed = reader.records;

        ENCODED_DATA_MASK mask = string_to_edm(parsed[0]);

        size_t expected_bytes = 4;
        size_t offset = 1;
        mnn_data_t mnn2;

        if(mask & MEDM_PID)
        {
            mnn2.pid = (uint32_t)strtoul(parsed[offset], NULL, 10);
            offset         += 1;
            expected_bytes += sizeof(mnn2.pid);
        }

        if(mask & MEDM_THREAD)
        {
            mnn2.thread_id  = (uint64_t)strtoul(parsed[offset], NULL, 10);
            offset         += 1;
            expected_bytes += sizeof(mnn2.thread_id);
        }

        if(mask & MEDM_TIMESTAMP)
        {
            mnn2.timestamp  = (uint64_t)strtoul(parsed[offset], NULL, 10);
            offset         += 1;
            expected_bytes += sizeof(mnn2.timestamp);
        }
        
        if(mask & MEDM_LINE)
        {
            mnn2.line = (uint32_t)strtoul(parsed[offset], NULL, 10);
            offset         += 1;
            expected_bytes += sizeof(mnn2.line);
        }
        
        if(mask & MEDM_SEVERITY)
        {
            mnn2.severity   = (uint8_t)strtoul(parsed[offset], NULL, 10);
            offset         += 1;
            expected_bytes += sizeof(mnn2.severity);
        }

        if(mask & MEDM_FILE)
        {
            mnn2.file_len   = (uint8_t)strlen(parsed[offset]);
            mnn2.file       = parsed[offset];
            offset         += 1;
            expected_bytes += sizeof(mnn2.file_len) + (size_t)mnn2.file_len;
        }
       
        if(mask & MEDM_FUNCTION)
        {
            mnn2.func_len   = (uint8_t)strlen(parsed[offset]),
            mnn2.func       = parsed[offset],
            offset         += 1;
            expected_bytes += sizeof(mnn2.func_len) + (size_t)mnn2.func_len;
        }

        if(mask & MEDM_MESSAGE)
        {
            mnn2.msg_len     = (uint16_t)strlen(parsed[offset]),
            mnn2.msg         = parsed[offset],
            offset         += 1;
            expected_bytes += sizeof(mnn2.msg_len) + (size_t)mnn2.msg_len;
        }
       
        bool ok = mm_encode(
            &mask,
            &mnn2,
            buffer,TEST_BUFFER_SIZE,
            &encoded_bytes
        );
        if(ok == false)
        {
            TRACE_ERROR_POSITION();
            TEST_ERROR("Iteration %d",record);
            TEST_ERROR("Encoding fail!");
            return 1;
        }
        
        if(encoded_bytes != expected_bytes)
        {
            TRACE_ERROR_POSITION();
            TEST_ERROR("Iteration %d",record);
            TEST_ERROR("Encoded bytes %ld differs from %ld",encoded_bytes,expected_bytes);
            return 1;
        }

        for(size_t tmp = 0 ; tmp < encoded_bytes ; tmp ++)
        {
            char *target = reader.records[tmp + offset];

            int expected_value = STRING_TO_INT(target);
            int computed_value = buffer[tmp];
            
            if (computed_value != expected_value)
            {
                TRACE_ERROR_POSITION();
                TEST_ERROR("Encoded byte differs from expected (tmp: %ld, record %d)", tmp, record);
                
                TEST_ERROR("(computed: %d [ASCII: '%c'], expected: %d [ASCII: '%c'])", 
                        computed_value, 
                        (computed_value >= 32 && computed_value <= 126) ? computed_value : '?', 
                        expected_value, 
                        (expected_value >= 32 && expected_value <= 126) ? expected_value : '?');
                TEST_ERROR("(%s)",target);
                return 1;
            }

            TEST_INFO("Ok computed %d, expected %d (ASCII char: '%c')",computed_value,expected_value,
                        (expected_value >= 32 && expected_value <= 126) ? expected_value : '?');
        }

        record++;
    }
    
    csvreader_close(&reader);

    return 0;
}
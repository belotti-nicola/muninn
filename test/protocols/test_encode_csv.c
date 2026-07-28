#include <internal/protocols/muninn_messages/muninn_codec.h>
#include <internal/protocols/muninn_messages/muninn_message.h>
#include <internal/protocols/muninn_messages/muninn_message_mask.h>


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

        muninn_message_mask mask = mmm_string_to_edm(parsed[0]);
        
        size_t expected_bytes = 6;
        size_t offset = 1;
        muninn_message mm = {0};
        muninn_payload payload = {0};
        muninn_header header = {0};
        
        if(mask & MEDM_PID)
        {
            payload.pid  = (uint32_t)strtoul(parsed[offset], NULL, 10);
            offset         += 1;
            expected_bytes += sizeof(payload.pid);
        }

        if(mask & MEDM_THREAD)
        {
            payload.thread_id  = (uint64_t)strtoul(parsed[offset], NULL, 10);
            offset               += 1;
            expected_bytes       += sizeof(payload.thread_id);
        }

        if(mask & MEDM_TIMESTAMP)
        {
            payload.timestamp  = (uint64_t)strtoul(parsed[offset], NULL, 10);
            offset               += 1;
            expected_bytes       += sizeof(payload.timestamp);
        }
        
        if(mask & MEDM_LINE)
        {
            payload.line = (uint32_t)strtoul(parsed[offset], NULL, 10);
            offset         += 1;
            expected_bytes += sizeof(payload.line);
        }
        
        if(mask & MEDM_SEVERITY)
        {
            payload.severity   = (uint8_t)strtoul(parsed[offset], NULL, 10);
            offset               += 1;
            expected_bytes       += sizeof(payload.severity);
        }

        if(mask & MEDM_FILE)
        {
            payload.file_len   = (uint8_t)strlen(parsed[offset]);
            payload.file       = parsed[offset];
            offset               += 1;
            expected_bytes       += sizeof(payload.file_len) + (size_t)payload.file_len;
        }
       
        if(mask & MEDM_FUNCTION)
        {
            payload.func_len   = (uint8_t)strlen(parsed[offset]),
            payload.func       = parsed[offset],
            offset               += 1;
            expected_bytes       += sizeof(payload.func_len) + (size_t)payload.func_len;
        }

        if(mask & MEDM_MESSAGE)
        {
            payload.msg_len  = (uint16_t)strlen(parsed[offset]),
            payload.msg      = parsed[offset],
            offset             += 1;
            expected_bytes     += sizeof(payload.msg_len) + (size_t)payload.msg_len;
        }

        encoded_bytes = expected_bytes;

        header.payload_len = expected_bytes + sizeof(header.payload_len);
        muninn_message_set(&mm,&header,&payload);
       
        bool ok = muninn_messages_encode(
            &mm,
            mask,
            buffer,&encoded_bytes
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

        TEST_INFO("Ok record is fine!Next one...");

        record++;

    }
    
    csvreader_close(&reader);

    return 0;
}
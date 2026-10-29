#include <internal/muninn_message.h>
#include <internal/protocols/muninn_encoded_messages/muninn_encoded_message.h>
#include <internal/protocols/muninn_encoded_messages/muninn_encoded_message_mask.h>
#include <internal/protocols/muninn_encoded_messages/muninn_encoded_message_codec.h>


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
        TRACE_ERROR_POSITION("Error");
        return 1;
    }

    int record = 0;
    while(csvreader_next(&reader))
    {
        size_t encoded_bytes = 0;

        char **parsed = reader.records;

        muninn_message_mask mask = mmm_string_to_edm(parsed[0]);
        
        size_t expected_bytes = 4;
        size_t offset = 1;
        
        muninn_message_t mm = {0};
        
        if(mask & MEDM_PID)
        {
            mm.pid  = (uint32_t)strtoul(parsed[offset], NULL, 10);
            offset         += 1;
            expected_bytes += sizeof(mm.pid);
        }

        if(mask & MEDM_THREAD)
        {
            mm.thread_id  = (uint64_t)strtoul(parsed[offset], NULL, 10);
            offset               += 1;
            expected_bytes       += sizeof(mm.thread_id);
        }

        if(mask & MEDM_TIMESTAMP)
        {
            mm.timestamp  = (uint64_t)strtoul(parsed[offset], NULL, 10);
            offset               += 1;
            expected_bytes       += sizeof(mm.timestamp);
        }
        
        if(mask & MEDM_LINE)
        {
            mm.line = (uint32_t)strtoul(parsed[offset], NULL, 10);
            offset         += 1;
            expected_bytes += sizeof(mm.line);
        }
        
        if(mask & MEDM_SEVERITY)
        {
            mm.severity   = (uint8_t)strtoul(parsed[offset], NULL, 10);
            offset               += 1;
            expected_bytes       += sizeof(mm.severity);
        }

        if(mask & MEDM_FILE)
        {
            mm.file_len   = (uint8_t)strlen(parsed[offset]);
            mm.file       = parsed[offset];
            offset               += 1;
            expected_bytes       += sizeof(mm.file_len) + (size_t)mm.file_len;
        }
       
        if(mask & MEDM_FUNCTION)
        {
            mm.func_len   = (uint8_t)strlen(parsed[offset]),
            mm.func       = parsed[offset],
            offset               += 1;
            expected_bytes       += sizeof(mm.func_len) + (size_t)mm.func_len;
        }

        if(mask & MEDM_MESSAGE)
        {
            mm.msg_len  = (uint16_t)strlen(parsed[offset]),
            mm.msg      = parsed[offset],
            offset             += 1;
            expected_bytes     += sizeof(mm.msg_len);
        }

        encoded_bytes = expected_bytes;
       
        bool ok = muninn_messages_encode(
            &mm,mask,buffer,TEST_BUFFER_SIZE,
            &encoded_bytes
        );
        if(ok == false)
        {
            TRACE_ERROR_POSITION("Iteration %d",record);
            TRACE_ERROR_POSITION("Encoding fail!");
            return 1;
        }
        
        if(encoded_bytes != expected_bytes)
        {
            TRACE_ERROR_POSITION("Iteration %d",record);
            TRACE_ERROR_POSITION("Encoded bytes %ld differs from %ld",encoded_bytes,expected_bytes);
            return 1;
        }

        for(size_t tmp = 0 ; tmp < encoded_bytes ; tmp ++)
        {
            char *target = reader.records[tmp + offset];

            int expected_value = STRING_TO_INT(target);
            int computed_value = buffer[tmp];
            
            if (computed_value != expected_value)
            {
                TRACE_ERROR_POSITION("Encoded byte differs from expected (tmp: %ld, record %d)", tmp, record);   
                TRACE_ERROR_POSITION("(computed: %d [ASCII: '%c'], expected: %d [ASCII: '%c'])", 
                        computed_value, 
                        (computed_value >= 32 && computed_value <= 126) ? computed_value : '?', 
                        expected_value, 
                        (expected_value >= 32 && expected_value <= 126) ? expected_value : '?');
                TRACE_ERROR_POSITION("(%s)",target);
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
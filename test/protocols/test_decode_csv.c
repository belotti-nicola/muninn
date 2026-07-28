#include <internal/protocols/muninn_messages/muninn_codec.h>
#include <internal/protocols/muninn_messages/muninn_message_mask.h>
#include <internal/protocols/muninn_messages/muninn_message.h>

#include "reader/csv_reader.h"
#include <stdlib.h>

#include <string.h>
#include <inttypes.h>

#include "test_utils.h"

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
        muninn_message_mask mask = mmm_string_to_edm(edm_st);

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
                TEST_ERROR("Buffer isn't big enough!");
                csvreader_close(&reader);
                return 1;
            }

            uint8_t val = (uint8_t)strtoul(reader.records[i], NULL, 0);
            buf[binary_len++] = val; 
        }

        muninn_header header;

        muninn_payload payload;
        uint8_t mnn_file_buf[256] = {0};
        uint8_t mnn_func_buf[256] = {0};
        uint8_t mnn_msg_buf[2048] = {0};

        payload.file = mnn_file_buf;
        payload.func = mnn_func_buf;
        payload.msg  = mnn_msg_buf;

        muninn_message mm = {0};
        muninn_message_set(&mm,&header,&payload);
        
        bool rc = muninn_messages_decode(buf,binary_len,&mm);
        if(rc == false)
        {
            TRACE_ERROR_POSITION();
            TEST_ERROR("mm_decode fail for iteration %d",iteration);
            return 1;
        }
       
        size_t header_expected_size = 4;
        size_t buffer_offset = 1;
        if( (mask & MEDM_PID) && reader.records_size >= buffer_offset)
        {
            int expected = STRING_TO_INT(reader.records[buffer_offset]);
            if(expected != payload.pid)
            {
                TRACE_ERROR_POSITION();
                TEST_ERROR("Iteration %d",iteration);
                TEST_ERROR("Computed pid is %d but %d was expected",payload.pid,expected);
                TEST_ERROR("(record %s)",reader.records[buffer_offset]);
                return 1;
            }
            buffer_offset += 1;
            header_expected_size += sizeof(payload.pid);
        }

        if( (mask & MEDM_THREAD) && reader.records_size >= buffer_offset)
        {
            char *target = reader.records[buffer_offset];
            uint64_t expected = STRING_TO_UINT64(target);
            if(expected != payload.thread_id)
            {
                TRACE_ERROR_POSITION();
                TEST_ERROR("Iteration %d",iteration);
                TEST_ERROR("Computed pid is %ld but %ld was expected",payload.thread_id,expected);
                TEST_ERROR("(record %s)",reader.records[buffer_offset]);
                return 1;
            }
            buffer_offset += 1;
            header_expected_size += sizeof(payload.thread_id);
        }

        if( (mask & MEDM_TIMESTAMP) && reader.records_size >= buffer_offset)
        {
            uint64_t expected = STRING_TO_UINT64(reader.records[buffer_offset]);
            if(expected != payload.timestamp)
            {
                TRACE_ERROR_POSITION();
                TEST_ERROR("Iteration %d",iteration);
                TEST_ERROR("Computed thread_id is %ld but %ld was expected",payload.timestamp,expected);
                TEST_ERROR("(record %s)",reader.records[buffer_offset]);
                return 1;
            }
            buffer_offset += 1;
            header_expected_size += sizeof(payload.timestamp);
        }

        if( (mask & MEDM_LINE) && reader.records_size >= buffer_offset)
        {
            char *target = reader.records[buffer_offset];
            int expected = STRING_TO_INT(target);
            if(expected != payload.line)
            {
                TRACE_ERROR_POSITION();
                TEST_ERROR("Iteration %d",iteration);
                TEST_ERROR("Computed line is %d but %d was expected",payload.line,expected);
                TEST_ERROR("(record %s)",reader.records[buffer_offset]);
                return 1;
            }
            buffer_offset += 1;
            header_expected_size += sizeof(payload.line);
        }

        if( (mask & MEDM_SEVERITY) && reader.records_size >= buffer_offset)
        {
            int expected = STRING_TO_INT(reader.records[buffer_offset]);
            if(expected != payload.severity)
            {
                TRACE_ERROR_POSITION();
                TEST_ERROR("Iteration %d",iteration);
                TEST_ERROR("Computed %d but %d was expected",payload.severity,expected);
                TEST_ERROR("(record %s)",reader.records[buffer_offset]);
                return 1;
            }
            buffer_offset += 1;
            header_expected_size += sizeof(payload.severity);
        }

        if( (mask & MEDM_FILE) && reader.records_size >= buffer_offset + 1)
        {
            int expected = strlen(reader.records[buffer_offset]);
            if(expected != payload.file_len)
            {
                TRACE_ERROR_POSITION();
                TEST_ERROR("Iteration %d",iteration);
                TEST_ERROR("Computed file_len is %d but %d was expected",payload.file_len,expected);
                TEST_ERROR("(record %s)",reader.records[buffer_offset]);
                return 1;
            }
            header_expected_size += sizeof(payload.file_len);

            for( int i = 0 ; i < payload.file_len ; i++)
            {
                char *target = reader.records[buffer_offset] + i;
                int expected = *target;
                if(expected != payload.file[i])
                {
                    TRACE_ERROR_POSITION();
                    TEST_ERROR("Iteration %d",iteration);
                    TEST_ERROR("Computed file byte %d-th is %d but %d was expected",i,payload.file[i],expected);
                    TEST_ERROR("(record %s)",reader.records[buffer_offset]);
                    return 1;
                }
            }
            buffer_offset += 1 ;
            header_expected_size += payload.file_len;
        }

        if( (mask & MEDM_FUNCTION) && reader.records_size >= buffer_offset + 1)
        {
            int expected = strlen(reader.records[buffer_offset]);
            if(expected != payload.func_len)
            {
                TRACE_ERROR_POSITION();
                TEST_ERROR("Iteration %d",iteration);
                TEST_ERROR("Computed func_len is %d but %d was expected",payload.func_len,expected);
                TEST_ERROR("(record %s)",reader.records[buffer_offset]);
                return 1;
            }
            header_expected_size += sizeof(payload.func_len);

            for( int i = 0 ; i < payload.func_len ; i++)
            {
                char *target = reader.records[buffer_offset] + i;
                int expected = *target;
                int computed = payload.func[i];
                if(expected != computed)
                {
                    TRACE_ERROR_POSITION();
                    TEST_ERROR("Iteration %d",iteration);
                    TEST_ERROR("Computed func byte %d-th is %d but %d was expected",i,computed,expected);
                    TEST_ERROR("(record %s)",target);
                    return 1;
                }
            }
            buffer_offset += 1 ;
            header_expected_size += payload.func_len;
        }

      
        if( (mask & MEDM_MESSAGE) && reader.records_size >= buffer_offset + 1)
        {
            int expected = strlen(reader.records[buffer_offset]);
            if(expected != payload.msg_len)
            {
                TRACE_ERROR_POSITION();
                TEST_ERROR("Iteration %d",iteration);
                TEST_ERROR("Computed file_len is %d but %d was expected",payload.msg_len,expected);
                TEST_ERROR("(record %s)",reader.records[buffer_offset]);
                return 1;
            }
            header_expected_size += sizeof(payload.msg_len);

            for( int i = 0 ; i < payload.msg_len ; i++)
            {
                char *tmp = reader.records[buffer_offset]+i;

                int expected = (int)*tmp;
                int computed = payload.msg[i];
                if(expected != computed)
                {
                    TRACE_ERROR_POSITION();
                    TEST_ERROR("Iteration %d",iteration);
                    TEST_ERROR("Computed msg byte %d-th is %d but %d was expected",i,computed,expected);
                    TEST_ERROR("(record  %s)",reader.records[buffer_offset]);
                    TEST_ERROR("(decoded %.*s)",payload.msg_len,payload.msg);
                    return 1;
                    
                }
            }
            buffer_offset += 1 ;
            header_expected_size += payload.msg_len;
        }
        
        if(header.payload_len != header_expected_size)
        {
            TRACE_ERROR_POSITION();
            TEST_ERROR("Iteration %d",iteration);
            TEST_ERROR("Computed file payload_len len %d is different from expected one %ld",(int)header.payload_len,buffer_offset);
            TEST_ERROR("(record %s)",reader.records[buffer_offset]);
            return 1;
        }

        iteration++;
    }
    
    return 0;
}
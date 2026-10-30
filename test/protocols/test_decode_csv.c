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
        TRACE_ERROR_POSITION("Error csvreader_open");
        return 1;
    }

    uint8_t buf[4096] = {0};
    uint8_t buffer_msg[1024] = {0};
    uint8_t buffer_fun[1024] = {0};
    uint8_t buffer_file[1024] = {0};

    
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
                TRACE_ERROR_POSITION("Buffer isn't big enough!");
                csvreader_close(&reader);
                return 1;
            }

            uint8_t val = (uint8_t)strtoul(reader.records[i], NULL, 0);
            buf[binary_len++] = val; 
        }

        muninn_message_t mm = {0};
        mm.file = buffer_file;
        mm.msg  = buffer_msg;
        mm.func = buffer_fun;

        if ( muninn_messages_decode(buf,binary_len,&mm) == false)
        {
            TRACE_ERROR_POSITION("mm_decode fail for iteration %d",iteration);
            return 1;
        }

        size_t buffer_offset = 1;
        
        if( (mask & MEDM_PID) && reader.records_size >= buffer_offset)
        {
            int expected = STRING_TO_INT(reader.records[buffer_offset]);
            if(expected != mm.pid)
            {
                TRACE_ERROR_POSITION("Iteration %d",iteration);
                TRACE_ERROR_POSITION("Computed pid is %d but %d was expected",mm.pid,expected);
                TRACE_ERROR_POSITION("(record %s)",reader.records[buffer_offset]);
                return 1;
            }
            buffer_offset += 1;
        }

        if( (mask & MEDM_THREAD) && reader.records_size >= buffer_offset)
        {
            char *target = reader.records[buffer_offset];
            uint64_t expected = STRING_TO_UINT64(target);
            if(expected != mm.thread_id)
            {
                TRACE_ERROR_POSITION("Iteration %d",iteration);
                TRACE_ERROR_POSITION("Computed pid is %ld but %ld was expected",mm.thread_id,expected);
                TRACE_ERROR_POSITION("(record %s)",reader.records[buffer_offset]);
                return 1;
            }
            buffer_offset += 1;
        }

        if( (mask & MEDM_TIMESTAMP) && reader.records_size >= buffer_offset)
        {
            uint64_t expected = STRING_TO_UINT64(reader.records[buffer_offset]);
            if(expected != mm.timestamp)
            {
                TRACE_ERROR_POSITION("Iteration %d",iteration);
                TRACE_ERROR_POSITION("Computed thread_id is %ld but %ld was expected",mm.timestamp,expected);
                TRACE_ERROR_POSITION("(record %s)",reader.records[buffer_offset]);
                return 1;
            }
            buffer_offset += 1;
        }

        if( (mask & MEDM_LINE) && reader.records_size >= buffer_offset)
        {
            char *target = reader.records[buffer_offset];
            int expected = STRING_TO_INT(target);
            if(expected != mm.line)
            {
                TRACE_ERROR_POSITION("Iteration %d",iteration);
                TRACE_ERROR_POSITION("Computed line is %d but %d was expected",mm.line,expected);
                TRACE_ERROR_POSITION("(record %s)",reader.records[buffer_offset]);
                return 1;
            }
            buffer_offset += 1;
        }

        if( (mask & MEDM_SEVERITY) && reader.records_size >= buffer_offset)
        {
            int expected = STRING_TO_INT(reader.records[buffer_offset]);
            if(expected != mm.severity)
            {
                TRACE_ERROR_POSITION("Iteration %d",iteration);
                TRACE_ERROR_POSITION("Computed %d but %d was expected",mm.severity,expected);
                TRACE_ERROR_POSITION("(record %s)",reader.records[buffer_offset]);
                return 1;
            }
            buffer_offset += 1;
        }

        if( (mask & MEDM_FILE) && reader.records_size >= buffer_offset + 1)
        {
            int expected = strlen(reader.records[buffer_offset]);
            if(expected != mm.file_len)
            {
                TRACE_ERROR_POSITION("Iteration %d",iteration);
                TRACE_ERROR_POSITION("Computed file_len is %d but %d was expected",mm.file_len,expected);
                TRACE_ERROR_POSITION("(record %s)",reader.records[buffer_offset]);
                return 1;
            }

            for( int i = 0 ; i < mm.file_len ; i++)
            {
                char *target = reader.records[buffer_offset] + i;
                int expected = *target;
                if(expected != mm.file[i])
                {
                    TRACE_ERROR_POSITION("Iteration %d",iteration);
                    TRACE_ERROR_POSITION("Computed file byte %d-th is %d but %d was expected",i,mm.file[i],expected);
                    TRACE_ERROR_POSITION("(record %s)",reader.records[buffer_offset]);
                    return 1;
                }
            }
            buffer_offset += 1 ;
        }

        if( (mask & MEDM_FUNCTION) && reader.records_size >= buffer_offset + 1)
        {
            int expected = strlen(reader.records[buffer_offset]);
            if(expected != mm.func_len)
            {
                TRACE_ERROR_POSITION("Iteration %d",iteration);
                TRACE_ERROR_POSITION("Computed func_len is %d but %d was expected",mm.func_len,expected);
                TRACE_ERROR_POSITION("(record %s)",reader.records[buffer_offset]);
                return 1;
            }

            for( int i = 0 ; i < mm.func_len ; i++)
            {
                char *target = reader.records[buffer_offset] + i;
                int expected = *target;
                int computed = mm.func[i];
                if(expected != computed)
                {
                    TRACE_ERROR_POSITION("Iteration %d",iteration);
                    TRACE_ERROR_POSITION("Computed func byte %d-th is %d but %d was expected",i,computed,expected);
                    TRACE_ERROR_POSITION("(record %s)",target);
                    return 1;
                }
            }
            buffer_offset += 1 ;
        }
  
        if( (mask & MEDM_MESSAGE) && reader.records_size >= buffer_offset + 1)
        {
            int expected = strlen(reader.records[buffer_offset]);
            if(expected != mm.msg_len)
            {
                TRACE_ERROR_POSITION("Iteration %d",iteration);
                TRACE_ERROR_POSITION("Computed file_len is %d but %d was expected",mm.msg_len,expected);
                TRACE_ERROR_POSITION("(record %s)",reader.records[buffer_offset]);
                return 1;
            }

            for( int i = 0 ; i < mm.msg_len ; i++)
            {
                char *tmp = reader.records[buffer_offset]+i;

                int expected = (int)*tmp;
                int computed = mm.msg[i];
                if(expected != computed)
                {
                    TRACE_ERROR_POSITION("Iteration %d",iteration);
                    TRACE_ERROR_POSITION("Computed msg byte %d-th is %d but %d was expected",i,computed,expected);
                    TRACE_ERROR_POSITION("(record  %s)",reader.records[buffer_offset]);
                    TRACE_ERROR_POSITION("(decoded %.*s)",mm.msg_len,mm.msg);
                    return 1;
                    
                }
            }
            buffer_offset += 1 ;
        }

        iteration += 1;
    }

    return 0;
}

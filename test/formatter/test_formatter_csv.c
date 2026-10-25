#include "test_utils.h"

#include <reader/csv_reader.h>

#include <string.h>

#include <internal/muninn_formatter.h>

#include <internal/protocols/muninn_encoded_messages/muninn_encoded_message.h>
#include <internal/protocols/muninn_encoded_messages/muninn_encoded_message_payload.h>
#include <internal/protocols/muninn_encoded_messages/muninn_encoded_message_header.h>

#define TEST_FILE_SIZE 100
#define EXPANDED_MAX_SIZE 1024

int main()
{
    char test_file[TEST_FILE_SIZE] = {0};

    if ( get_test_file_data_path(test_file,TEST_FILE_SIZE,"/data/test_formatter_csv_data.csv") == NULL )
    {
        TRACE_ERROR_POSITION("Error: could not get test data file.");
        return 1;
    }

    CSVReader reader;

    bool rc = csvreader_open(&reader,test_file);
    if(rc == false)
    {   
        TRACE_ERROR_POSITION("Error: could not open %s",test_file);
        return 1;
    }

    
    muninn_message   mm;
    muninn_payload   mp;
    muninn_header    mh;

    mm.header  = &mh;
    mm.payload = &mp;

    muninn_formatter_t formatter = {0};

    char *pattern;
    char *expected;
    size_t expanded_size;

    char expanded[EXPANDED_MAX_SIZE] = {0};

    int iteration = 0;   
    while ( csvreader_next( &reader ) != false )
    {
        if ( reader.records_size == 0 )
        {
            TRACE_ERROR_POSITION("Error at %ld iteration: records size is 0.",reader.records_size);
            return 1;
        }

        pattern = reader.records[0];
        size_t pattern_size = strlen(pattern);

        if ( muninn_formatter_init( &formatter,pattern,strlen(pattern) ) == false )
        {
            TEST_INFO("NOK muninn_formatter_init");
            return 1;
        }

        size_t expected_tokens = count_occurencies(pattern,pattern_size,'<');
        size_t non_text_tokens = 0;
        for ( int i = 0 ; i < formatter.tokens_size ; i++ )
        {
            formatter_token_type_t type = formatter.tokens[ i ].type;
            if ( type == M_TEXT ) continue;

            non_text_tokens += 1;
        }

        if ( expected_tokens + 2 ==  non_text_tokens )
        {
            TRACE_ERROR_POSITION("Error at %d iteration: expected tokens %ld + 2 differs from non text_tokens %d",
                iteration,
                non_text_tokens,
                count_occurencies(pattern,pattern_size,'<'));
            return 1;
        }

        size_t record_i = 1;
        for ( int i = 0 ; i < formatter.tokens_size ; i++ )
        {
            formatter_token_type_t type = formatter.tokens[ i ].type;
            if ( type == M_TEXT ) continue;

            if ( type == M_TIMESTAMP )
            {
                mp.timestamp = STRING_TO_UINT64(reader.records[record_i]);
            }
            else if ( type == M_THREAD_ID ) 
            {
                mp.thread_id = STRING_TO_UINT64(reader.records[record_i]);
            }
            else if ( type == M_LINE )
            {
                mp.line = STRING_TO_INT(reader.records[record_i]);
            }
            else if ( type == M_SEVERITY )
            {
                mp.severity = STRING_TO_INT(reader.records[record_i]);
            }
            else if ( type == M_PID )
            {
                mp.pid = STRING_TO_INT(reader.records[record_i]);
            }
            else if ( type == M_FILE )
            {
                mp.file     = reader.records[record_i];
                mp.file_len = strlen(reader.records[record_i]);
            }
            else if ( type == M_FUNCTION)
            {
                mp.func     = reader.records[record_i];
                mp.func_len = strlen(reader.records[record_i]);
            }
            else if ( type == M_MESSAGE )
            {
                mp.msg     = reader.records[record_i];
                mp.msg_len = strlen(reader.records[record_i]);   
            } else 
            {
                TRACE_ERROR_POSITION("Token error!");
                return 1;
            }

            record_i += 1;            
        }

        expected = reader.records[record_i];
       
        if ( muninn_formatter_expand( &formatter,mm,expanded,&expanded_size,EXPANDED_MAX_SIZE) == false )
        {
            TEST_INFO("NOK at iteration %d: muninn_formatter_expand",iteration);
            return 1;
        }

        if ( expanded_size != strlen(expected) )
        {
            TRACE_ERROR_POSITION("Error at iteration %d: expanded size (%ld) is different from expected one (%ld).",iteration,expanded_size,strlen(expected));
            return 1;
        }

        for ( int i = 0 ; i < expanded_size ; i++ )
        {
            if ( expected[ i ] != expanded[ i ] )
            {
                TRACE_ERROR_POSITION("Error at iteration %d: expected '%c', found '%c' instead.",iteration,expected[ i ], expanded[ i ]);
                return 1;
            }
        }       

        iteration += 1;
    }

    return 0;
}
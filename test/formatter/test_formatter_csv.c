#include "test_utils.h"

#include <reader/csv_reader.h>

#include <string.h>

#include <internal/muninn_formatter.h>

#include <internal/protocols/muninn_messages/muninn_message.h>
#include <internal/protocols/muninn_messages/muninn_message_payload.h>
#include <internal/protocols/muninn_messages/muninn_message_header.h>

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
    int tokens;

    char expanded[EXPANDED_MAX_SIZE] = {0};

    int iteration = 0;   
    while ( csvreader_next( &reader ) != false )
    {
        if ( reader.records_size < 11 )
        {
            TRACE_ERROR_POSITION("Error at %d iteration: records size is %ld (less than 17).",iteration,reader.records_size);
            return 1;
        }

        mp.timestamp = STRING_TO_UINT64(reader.records[0]);
        mp.thread_id = STRING_TO_UINT64(reader.records[1]);
        mp.line = STRING_TO_INT(reader.records[2]);
        mp.severity = STRING_TO_INT(reader.records[3]);
        mp.pid = STRING_TO_INT(reader.records[4]);
        mp.file = reader.records[5];
        mp.file_len = strlen(reader.records[5]);
        mp.func = reader.records[6];
        mp.func_len = strlen(reader.records[6]);
        mp.msg = reader.records[7];
        mp.msg_len = strlen(reader.records[7]);

        pattern = reader.records[8];

        tokens = STRING_TO_INT(reader.records[9]);
        expected = reader.records[10];

        if ( muninn_formatter_init( &formatter,pattern,strlen(pattern) ) == false )
        {
            TEST_INFO("NOK muninn_formatter_init");
            return 1;
        }

        if ( muninn_formatter_expand( &formatter,mm,expanded,&expanded_size,EXPANDED_MAX_SIZE) == false )
        {
            TEST_INFO("NOK muninn_formatter_expand");
            return 1;
        }

        if ( formatter.tokens_size != tokens )
        {
            TEST_INFO("NOK muninn_formatter_expand: wrong number of tokens (found %ld instead of %d)",formatter.tokens_size,tokens);
            return 1;
        }

        if ( expanded_size != strlen(expected) )
        {
            TRACE_ERROR_POSITION("Error: expanded size (%ld) is different from expected one (%ld).",expanded_size,strlen(expected));
            return 1;
        }

        for ( int i = 0 ; i < expanded_size ; i++ )
        {
            if ( expected[ i ] != expanded[ i ] )
            {
                TRACE_ERROR_POSITION("Error at iteration %d: expected '%c', found '%c' instead.",i,expected[ i ], expanded[ i ]);
                return 1;
            }
        }       

        iteration += 1;
    }

    return 0;
}
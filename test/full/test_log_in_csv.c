#include <internal/protocols/muninn_messages/muninn_codec.h>
#include <internal/protocols/muninn_messages/muninn_message.h>
#include <internal/protocols/muninn_messages/muninn_message_mask.h>

#include <muninn.h>


#include "test_utils.h"
#include "reader/csv_reader.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#define TEST_FILE_SIZE 256
#define TEST_BUFFER_SIZE 4096

int main()
{
    char test_samples_path[2048];
    if ( get_test_file_data_path(test_samples_path,2048,"/data/test_log_in_csv_data.csv") == NULL )
    {
        TRACE_ERROR_POSITION("Error: could not compute test data file");
        return 1;
    }

    char test_file[TEST_FILE_SIZE] = {0};
    if ( compute_test_file_name(test_file,TEST_FILE_SIZE) == NULL )
    {
        TRACE_ERROR_POSITION("Error: could not compute test file path");
        return 1;
    }
    TEST_INFO("test_file: '%s'",test_file);

    uint8_t buffer[TEST_BUFFER_SIZE];
      
    CSVReader reader;
    bool rc = csvreader_open(&reader,test_samples_path);
    if(rc == false)
    {   
        TRACE_ERROR_POSITION("Error: could not open %s",test_samples_path);
        return 1;
    }

    CONFIG CONF;
    muninn_config_default(&CONF);
    muninn_config_set_file(&CONF,true,test_file);
    muninn_config_set_mask(&CONF,MEDM_NONE);

    muninn_t muninn;
    muninn_init(&muninn,&CONF);


    int record = 0;
    while(csvreader_next(&reader))
    {
        char *test_message  = reader.records[0];
        char *test_mask_str = reader.records[1];

        TEST_INFO("records_size %ld",reader.records_size);

        muninn_message_mask mem = mmm_string_to_edm(test_mask_str);
        TEST_INFO("%d",(int)mem);

        muninn_config_set_mask(&CONF,mem);

        muninn_log_fatal(&muninn,test_message);

        csvreader_next(&reader);
    }

    muninn_shutdown(&muninn);
    
    csvreader_close(&reader);

    TEST_INFO("OK");

    return 0;
}
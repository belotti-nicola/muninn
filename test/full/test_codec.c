#include "muninn.h"


#include <assert.h>
#include <string.h>
#include <unistd.h>

#define SIZE 200

#include "test_utils.h"

int main(void)
{
    char logfile[SIZE];
    if(compute_test_file_name(logfile,SIZE) == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Cannot compute test file");
        return 1;
    }
    
    CONFIG CONF;
    CONF.mask = MEDM_ALL;
    
    muninn_config_default(&CONF);
    muninn_config_set_file(&CONF,true,logfile);

    muninn_t muninn;
    muninn_init(&muninn,CONF);

    muninn_log_fatal(&muninn, "hello");
    muninn_log_fatal(&muninn, "world");

    muninn_shutdown(&muninn);

    FILE *test_file = fopen(logfile, "r");
    if(test_file == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: char pointer is null for file %s.",logfile);
        return 1;
    }

    char buffer[256];
    size_t size;

    if(fgets(buffer, 256, test_file) == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: fgets is null for file %s.",logfile);
        return 1;
    }

    
    
    /*
    if(strncmp(buffer,"hello",strlen("hello")) != 0)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: buffer(%s) is not substring of the expected(%s)",buffer,"hello");
        return 1;
    }
    */
    

    fclose(test_file);
    return 0;
}
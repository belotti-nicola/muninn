#include "muninn.h"

#include <assert.h>
#include <string.h>
#include <unistd.h>

#define SIZE 200

#include "test_utils.h"

int main(void)
{
    char log[SIZE];
    if(compute_test_file_name(log,SIZE) == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Cannot compute test file");
        return 1;
    }
    
    CONFIG CONF;
    muninn_config_default(&CONF);
    muninn_config_set_file(&CONF,true,log);

    muninn_t muninn;
    muninn_init(&muninn,CONF);

    muninn_log_fatal(&muninn, "hello");
    muninn_log_fatal(&muninn, "world");

    muninn_shutdown(&muninn);

    FILE *test_file = fopen(log, "r");
    if(test_file == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: char pointer is null for file %s.",log);
        return 1;
    }

    char buffer[256];
    size_t size;

    if(fgets(buffer, 256, test_file) == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: fgets is null for file %s.",log);
        return 1;
    }
    if(strncmp(buffer,"hello",strlen("hello")) != 0)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: buffer(%s) is not substring of the expected(%s)",buffer,"hello");
        return 1;
    }

    if(fgets(buffer, 256, test_file) == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: fgets is null for file %s.",log);
        return 1;
    }
    if(strncmp(buffer,"world",strlen("world")) != 0)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: buffer(%s) is not substring of the expected(%s)",buffer,"world");
        return 1;
    }

    if (fgets(buffer, sizeof(buffer), test_file) == NULL)
    {
        if (feof(test_file))
        {
            
        }
        else if (ferror(test_file))
        {
            return 1;
        }
    }

    fclose(test_file);
    return 0;
}
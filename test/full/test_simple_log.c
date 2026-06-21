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
    
    muninn_t muninn = {0};
    muninn_init(&muninn,log);

    muninn_log_fatal(&muninn, "hello");
    muninn_log_fatal(&muninn, "world");

    muninn_shutdown(&muninn);

    FILE *test_file = fopen(log, "r");
    if(test_file == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: char pointer is null at line %d (file %s).\n",__LINE__,log);
        return 1;
    }

    char buffer[256];
    char *res;

    res = fgets(buffer, sizeof(buffer), test_file);
    if(res == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: char pointer is null at line %d.\n",__LINE__);
        return 1;
    }
    if(strstr(buffer, "hello") == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: buffer(%s) is not substring of the expected(%s) at line %d.\n",buffer,"hello",__LINE__);
        return 1;
    }

    res = fgets(buffer, sizeof(buffer), test_file);
    if(res == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: char pointer is null at line %d.\n",__LINE__);
        return 1;
    }
    if(strstr(buffer, "world") == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: buffer(%s) is not substring of the expected(%s) at line %d.\n",buffer,"world",__LINE__);
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
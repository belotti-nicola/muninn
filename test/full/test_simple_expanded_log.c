#include "muninn.h"

#include <assert.h>
#include <string.h>
#include <unistd.h>

#define SIZE 200

#define PATTERN   "[<file>:<line>:<sev>]<msg>"

#include "test_utils.h"

int main(void)
{
    char log[SIZE];
    if(compute_test_file_name(log,SIZE) == NULL)
    {
        TRACE_ERROR_POSITION("Cannot compute test file");
        return 1;
    }
    TEST_INFO("Log file is: %s",log);
    
    CONFIG CONF;
    muninn_config_default(&CONF);
    muninn_config_set_file(&CONF,true,log);
    muninn_config_set_mask(&CONF,MEDM_SEVERITY|MEDM_MESSAGE|MEDM_LINE|MEDM_FILE);
    muninn_config_set_formatter(&CONF,PATTERN);

    muninn_t muninn;
    muninn_init(&muninn,&CONF);

    char expected[SIZE * 2];
    int line = __LINE__ + 1;
    muninn_log_fatal(&muninn, "hello");
    
    muninn_shutdown(&muninn);

    FILE *test_file = fopen(log, "r");
    if  ( test_file == NULL)
    {
        TRACE_ERROR_POSITION("Error: char pointer is null for file %s.",log);
        return 1;
    }
    
    char buffer[1024];
    if(fgets(buffer, 1024, test_file) == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error: fgets is null for file %s.",log);
        return 1;
    }

    snprintf(expected, sizeof expected, "[%s:%10d:%3d]hello\n", __FILE__, line,5);
    if (strcmp(buffer, expected) != 0) 
    {
        TRACE_ERROR_POSITION("Error: computed is different from expected one");
        TEST_ERROR("got:      %s", buffer);
        TEST_ERROR("expected: %s", expected);
        fclose(test_file);
        return 1;
    }

    fclose(test_file);
    TEST_INFO("TEST SUCEESS!");
    return 0;
}
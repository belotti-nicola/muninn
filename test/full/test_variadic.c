#include "muninn.h"

#include <assert.h>
#include <string.h>
#include <unistd.h>

#define SIZE 100

#include "test_utils.h"

int main(void)
{
    char log[SIZE];
    if(compute_test_file_name(log,SIZE) == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Error computing test file name in %d bytes",SIZE);
        return -1;
    }

    CONFIG CONF;
    muninn_config_default(&CONF);
    muninn_config_set_file(&CONF,true,log);

    muninn_t muninn;
    muninn_init(&muninn,CONF);

    muninn_log_info(&muninn,"%s","Hello");

    muninn_shutdown(&muninn);
    
    return 0;
}
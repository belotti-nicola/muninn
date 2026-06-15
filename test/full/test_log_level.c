#include "muninn.h"

#include <assert.h>
#include <string.h>
#include <unistd.h>

#include "test_utils.h"

#define PATH_SIZE 1024

int main(void)
{
    char file[PATH_SIZE];
    
    if( compute_test_file_name(file,PATH_SIZE) == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Could not compute test file name with %d bytes!",PATH_SIZE);
        return -1;
    }

    muninn_t muninn;
    muninn_init(&muninn,file);

    muninn_log_warning(&muninn,"Hello World");//LOGGED
    muninn_log_warning(&muninn,"Hello World");//LOGGED
    muninn_log_warning(&muninn,"Hello World");//LOGGED
    muninn_log_warning(&muninn,"Hello World");//LOGGED

    sleep(1);
    muninn_set_dynamic_level(&muninn,LOG_FATAL);
    sleep(1);

    muninn_log_warning(&muninn,"Hello World");//LOST
    muninn_log_warning(&muninn,"Hello World");//LOST
    muninn_log_warning(&muninn,"Hello World");//LOST


    sleep(1);
    muninn_shutdown(&muninn);

    FILE *ptr = fopen(file,"r");
    if( ptr == NULL )
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Could not open test file %s !",file);
        return 1;
    }

    int lines = 0;
    char ch;
    while ((ch = fgetc(ptr)) != EOF)
    {
        if (ch == '\n')
        lines++;
    }
    fclose(ptr);
    if(lines != 4)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Counted lines(%d) differs from expected(%d)!",lines,1);
        return 1;
    }
    
    return 0;
}
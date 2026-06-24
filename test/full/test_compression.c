#include "muninn.h"

#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <libgen.h>

#define PATH_LEN 100

#define RERUN_TIMES 1
#define TIMES       5300
#define BUFFER_SIZE 201

#include <libgen.h>
#include <unistd.h>
#include "test_utils.h"


int main(void)
{
    char filepath[PATH_LEN] = {0};
    if(compute_test_file_name(filepath,PATH_LEN) == NULL)
    {
        printf("Error: could not compute log test file\n");
        return 1;
    }

    CONFIG CONF;
    muninn_config_default(&CONF);
    muninn_config_set_file(&CONF,true,filepath);
    muninn_config_set_compressor(&CONF,true);

    muninn_t muninn;
    muninn_init(&muninn,CONF);

    char message[BUFFER_SIZE];
    char a = 'a';
    memset(message,a,BUFFER_SIZE-1);
    message[BUFFER_SIZE-1]='\0';

    for(int i=0;i<RERUN_TIMES;i++)
    {
        // overflow the F_MAX_SIZE limit!
        for(int j=0;j<TIMES;j++)
        {
            muninn_log_info(&muninn,message);
            usleep(5000);
        }
    }

    usleep(200);
    muninn_shutdown(&muninn);

    char cwd[PATH_LEN];
    if(get_executable_directory(cwd,PATH_LEN) == NULL)
    {
        printf("Error: could not detect executable directory\n");
        return 1;
    }
    int file_counter = files_counter(cwd);
    if ( file_counter != RERUN_TIMES) 
    {
        printf("Error: lz4 files number differs from expected (found %d files instead of %d).\n",file_counter,RERUN_TIMES);
        return 1;
    }

    return 0;
}
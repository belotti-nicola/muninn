#include "muninn.h"

#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#define RERUN_TIMES 3
#define TEST_LOG "test_compression.log"

#include <libgen.h>

int lz4_files_counter(const char* path)
{
    DIR* dir = opendir(path);
    if (!dir) return -1;

    struct dirent* entry;
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        const char* name = entry->d_name;
        size_t len = strlen(name);

        if (len > 4 && strcmp(name + len - 4, ".lz4") == 0)
        {
            count++;
        }
    }

    closedir(dir);
    return count;
}

int main(void)
{
    muninn_t muninn;
    muninn_init(&muninn,TEST_LOG);


    char message[LOG_MESSAGE_SIZE];
    int a_ascii = 'a' - '0';
    memset(message,a_ascii,LOG_MESSAGE_SIZE-1);
    message[LOG_MESSAGE_SIZE-1]='\0';

    int times = F_MAX_SIZE / LOG_MESSAGE_SIZE + 10;
    for(int i=0;i<RERUN_TIMES;i++)
    {
        // overflow the F_MAX_SIZE limit!
        for(int j=0;j<times;j++)
        {
            muninn_log_info(&muninn,message);
            usleep(10);
        }
    }
    muninn_shutdown(&muninn);

    int file_counter = lz4_files_counter(".");
    if ( file_counter != RERUN_TIMES) 
    {
        printf("Error: lz4 files number differs from expected (found %d files instead of %d).\n",file_counter,RERUN_TIMES);
        return 1;
    }

    return 0;
}
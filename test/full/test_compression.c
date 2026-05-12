#include "muninn.h"

#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#define RERUN_TIMES 1
#define TEST_LOG "test_compression.log"


int zst_files_counter(const char* path)
{
    DIR* dir = opendir(path);
    if (!dir) return -1;

    struct dirent* entry;
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        const char* name = entry->d_name;
        size_t len = strlen(name);

        if (len > 4 && strcmp(name + len - 4, ".zst") == 0) {
            count++;
        }
    }

    closedir(dir);
    return count;
}

int main(void)
{
    muninn_t muninn;
    muninn_init(TEST_LOG, &muninn);


    char message[M_SIZE];
    int a_ascii = 'a' - '0';
    memset(message,a_ascii,M_SIZE-1);
    message[M_SIZE-1]='\0';

    int times = F_MAX_SIZE / (M_SIZE-1) + 100;
    for(int i=0;i<RERUN_TIMES;i++)
    {
        // overflow the F_MAX_SIZE limit!
        for(int i=0;i<times;i++)
        {
            muninn_log(&muninn,message);
            usleep(10);
        }
    }
    muninn_shutdown(&muninn);

    int file_counter = zst_files_counter(".");
    if ( file_counter != 1) 
    {
        printf("Error: zst files number differs from expected (found %d files instead of %d).\n",file_counter,1);
        return 1;
    }

    return 0;
}
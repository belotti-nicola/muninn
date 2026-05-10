#include "muninn.h"

#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#define RERUN_TIMES 5

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
    muninn_init("test_compression.log", &muninn);


    char message[M_SIZE];
    int a_ascii = 'a' - '0';
    memset(message,a_ascii,M_SIZE-1);
    message[M_SIZE-1]='\0';

    int file_counter;
    int times = F_MAX_SIZE / (M_SIZE-1) + 50;
    for(int i=0;i<RERUN_TIMES;i++)
    {
        // overflow the F_MAX_SIZE limit!
        for(int i=0;i<times;i++)
        {
            muninn_log(&muninn,message);
            usleep(10);
        }

        file_counter = zst_files_counter(".");
        if ( file_counter != i+1) 
        {
            printf("Error: zst files number differs from expected (found %d files instead of %d).\n",file_counter,i+1);
            return 1;
        }

    }

   
    muninn_shutdown(&muninn);
    return 0;
}
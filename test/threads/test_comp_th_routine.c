#include <internal/compressor_th.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <internal/muninn_worker.h>
#include <internal/compressor_th.h>
#include <internal/ts_ring_buffer.h>
#include "test_utils.h"

int lz4_files_counter()
{
    DIR* dir = opendir(".");
    if (!dir) return -1;

    struct dirent* entry;
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        const char* name = entry->d_name;
        size_t len = strlen(name);

        if (len > 4 && strcmp(name + len - 4, ".lz4") == 0) {
            count++;
        }
    }

    closedir(dir);
    return count;
}
  

#define PATH_SIZE 100
#define MESSAGE_SIZE 100
#define QUEUE_SIZE 5

int main()
{
    char test_file[PATH_SIZE];
    if(compute_test_file_name(test_file,PATH_SIZE) == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Could not compute test file name.");
        return 1;
    }
    FILE* f = fopen(test_file,"w");
    if(f == NULL)
    {
        TRACE_ERROR_POSITION();
        TEST_ERROR("Could not create test file.");
        return 1;
    }
    fclose(f);f=NULL;
 

    int offset = 0;
    char buffer[MESSAGE_SIZE * QUEUE_SIZE];
    queue_message_t messages[QUEUE_SIZE] = {0};
    for (int i = 0; i < QUEUE_SIZE; i++)
    {
        setup_queue_message(messages + i, buffer + offset, MESSAGE_SIZE);
        offset += MESSAGE_SIZE;
    }

    ts_queue_t tsq;
    ts_queue_setup(&tsq, messages, QUEUE_SIZE);

    compressor_th_data data = {0};
    data.q = &tsq;

    muninn_worker_t mw = {0};
    mw_init(&mw,"test_worker",
        fcompressor_loop_fn,
        fcompressor_stop_fn,
        fcompressor_post_fn,
        (void *)&data
    );
    mw_start(&mw);
    
    mw_post(&mw,test_file,strlen(test_file));

    mw_shutdown(&mw);

    sleep_ms(10);
    if(mw_running(&mw) == true)
    {
        mw_shutdown(&mw);
        TRACE_ERROR_POSITION();
        TEST_ERROR("Running boolean is true instead of false at the end of the test.");
        return 1;
    }


    return 0;
}
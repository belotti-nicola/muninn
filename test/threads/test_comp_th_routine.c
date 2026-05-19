#include <internal/compressor_th.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#define MESSAGE_SIZE 100
#define QUEUE_SIZE 5
#define TESTPATH "test.log"

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

int main()
{
    int offset = 0;
    char buffer[MESSAGE_SIZE * QUEUE_SIZE]; 
    queue_message_t messages[QUEUE_SIZE] = {0};
    for(int i=0;i<QUEUE_SIZE;i++)
    {
        setup_queue_message(messages+i,buffer+offset,MESSAGE_SIZE);
        offset += MESSAGE_SIZE;
    }

    ts_queue_t tsq;
    ts_queue_setup(&tsq,messages,QUEUE_SIZE);

    compressor_th_data data;data.tasks = &tsq;

    FILE* f = fopen(TESTPATH,"w");
    if(f == NULL)
    {
        printf("Error: cannot open file %s\n",TESTPATH);
        return 1;
    }
    fclose(f);
    
    compressor_th_start(&data);
    compressor_th_perform(&data,TESTPATH);
    compressor_th_stop(&data);
    compressor_th_join(&data);

    int counter = lz4_files_counter();
    if( counter != 1)
    {
        printf("Error: file counter(%d) differs from expected(%d)\n",counter,1);
        return 1;
    }
    
    return 0;
}